"""Render the R36SX documentary; all large outputs stay in an explicit work dir."""
import argparse
import hashlib
import io
import json
import math
from pathlib import Path
import re
import subprocess
import textwrap

from PIL import Image, ImageDraw, ImageFont, ImageOps
from package_youtube import DEFAULT_OUTPUT, package as package_youtube

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
FPS, WIDTH, HEIGHT = 30, 1920, 1080
INK, MUTED, GREEN, GOLD = '#edf3f4', '#a8b8c4', '#92e6ca', '#ffc575'
NAME = 'R36SX-From-Stock-Firmware-to-DOS-English'


def run(command, log=None):
    result = subprocess.run(command, capture_output=True)
    if log:
        Path(log).write_bytes(result.stdout + result.stderr)
    if result.returncode:
        raise RuntimeError(result.stderr.decode('utf-8', errors='replace')[-5000:])
    return result.stdout


def probe(path):
    return json.loads(run(['ffprobe', '-v', 'error', '-show_format', '-show_streams', '-of', 'json', str(path)]))


def seconds(path):
    return float(probe(path)['format']['duration'])


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as source:
        for block in iter(lambda: source.read(4194304), b''):
            h.update(block)
    return h.hexdigest()


def font(size, bold=False, mono=False):
    return ImageFont.truetype('C:/Windows/Fonts/' + ('consola.ttf' if mono else 'seguisb.ttf' if bold else 'segoeui.ttf'), size)


def wrap(draw, text, xy, width, size=32, fill=INK, bold=False, mono=False, spacing=11):
    face, lines = font(size, bold, mono), []
    for paragraph in text.split('\n'):
        line = ''
        for word in paragraph.split():
            test = (line + ' ' + word).strip()
            if line and draw.textlength(test, font=face) > width:
                lines.append(line)
                line = word
            else:
                line = test
        lines.append(line)
    x, y = xy
    for line in lines:
        draw.text((x, y), line, font=face, fill=fill)
        y += size + spacing
    return y


def numbered_point(draw, number, text, *, number_x, text_x, y, width,
                   size, number_size, bold=False, spacing=11):
    # Different font families/sizes need the same baseline, not the same top.
    baseline = y + font(size, bold).getmetrics()[0]
    draw.text((number_x, baseline), f'{number:02}',
              font=font(number_size, mono=True), fill=GOLD, anchor='ls')
    return wrap(draw, text, (text_x, y), width, size,
                bold=bold, spacing=spacing)


def aligned_row(draw, cells, *, top, height):
    # Center the row's visible glyphs while sharing one baseline across columns.
    bounds = [draw.textbbox((0, 0), text, font=face, anchor='ls')
              for x, text, face, color in cells]
    ink_top = min(box[1] for box in bounds)
    ink_bottom = max(box[3] for box in bounds)
    baseline = round(top + height / 2 - (ink_top + ink_bottom) / 2)
    for x, text, face, color in cells:
        draw.text((x, baseline), text, font=face, fill=color, anchor='ls')


def canvas(scene, index):
    im = Image.new('RGB', (WIDTH, HEIGHT))
    d = ImageDraw.Draw(im)
    for y in range(HEIGHT):
        t = y / HEIGHT
        d.line((0, y, WIDTH, y), fill=(int(15 + 4*t), int(25 + 7*t), int(38 + 8*t)))
    d.rectangle((60, 44, 68, 128), fill=GREEN)
    label_baseline = 39 + font(24, mono=True).getmetrics()[0]
    d.text((92, label_baseline), f'{index:02d} / R36SX FIELD NOTES',
           font=font(24, mono=True), fill=GREEN, anchor='ls')
    d.text((92, 79), scene['title'], font=font(45, bold=True), fill=INK)
    date = scene['date']
    date_width = d.textlength(date, font=font(20, mono=True))
    d.text((1860-date_width, label_baseline), date,
           font=font(20, mono=True), fill=MUTED, anchor='ls')
    d.line((60, 145, 1860, 145), fill='#344859', width=2)
    d.rectangle((0, 962, WIDTH, HEIGHT), fill='#09121e')
    return im, d


def sidebar(d, scene):
    y = wrap(d, scene['panel_title'], (1390, 190), 455, 24, GREEN, mono=True, spacing=9) + 38
    for i, point in enumerate(scene['points'], 1):
        y = numbered_point(d, i, point, number_x=1390, text_x=1440,
                           y=y-3, width=405, size=29, number_size=22,
                           bold=True, spacing=9) + 34
    d.line((1390, 800, 1855, 800), fill='#344859', width=2)
    wrap(d, 'EVIDENCE', (1390, 821), 460, 19, GREEN, mono=True)
    end = wrap(d, scene['evidence'], (1390, 855), 455, 22, MUTED, spacing=7)
    assert end < 960, (scene['id'], 'Evidence panel overflow', end)


def still(source, timestamp):
    data = run(['ffmpeg', '-v', 'error', '-ss', str(timestamp), '-reinit_filter', '0',
                '-i', str(source), '-frames:v', '1', '-f', 'image2pipe', '-c:v', 'mjpeg', '-'])
    return Image.open(io.BytesIO(data)).convert('RGB')


def card(scene, kind, index, hero, output):
    im, d = canvas(scene, index)
    if kind in ['opening', 'closing']:
        crop = ImageOps.fit(hero, (680, 730), Image.Resampling.LANCZOS, centering=(0.5, 0.52))
        im.paste(crop, (60, 182))
        x = 820
        wrap(d, 'REVERSE ENGINEERING', (x, 200), 1000, 29, GREEN, mono=True)
        if kind == 'opening':
            wrap(d, 'From stock\nfirmware to DOS.', (x, 280), 1020, 79, bold=True, spacing=7)
            wrap(d, 'The R36SX / SF3000 project', (x, 505), 990, 38, MUTED)
            wrap(d, 'Firmware analysis. Native tools.\nA PC emulator on a MIPS handheld.', (x, 590), 1010, 32, spacing=12)
            d.text((x, 805), 'A project by Vitaliy Rudik', font=font(33, bold=True), fill=GOLD)
        else:
            wrap(d, 'Inspect. Test.\nDocument. Repeat.', (x, 280), 1010, 72, bold=True, spacing=8)
            y = 510
            for point in scene['points']:
                y = wrap(d, point, (x, y), 1020, 27, MUTED, spacing=8) + 17
            wrap(d, 'github.com/Witali/r36sx_disasm', (x, 827), 1030, 31, GREEN)
    elif kind == 'firmware':
        wrap(d, 'Harmless names.\nReal boot components.', (65, 190), 1750, 67, bold=True, spacing=9)
        rows = [('advapi32.dll', 'Linux kernel / U-Boot uImage'), ('ApplicationFrame.dll', 'AVP firmware / U-Boot uImage'), ('Bubbles.scr', 'Flattened device tree')]
        for i, (name, actual) in enumerate(rows):
            y = 410+i*132
            d.rounded_rectangle((65, y, 1855, y+105), radius=14, fill='#203247', outline='#3a5366', width=2)
            aligned_row(d, [(95, name, font(32, mono=True), GOLD),
                            (740, '→', font(34), GREEN),
                            (815, actual, font(32, bold=True), INK)],
                        top=y, height=105)
        d.text((70, 809), 'MIPS architecture — also used in home routers / little-endian ELF32', font=font(29), fill=MUTED)
        aligned_row(d, [(70, 'Only ~42 MiB usable RAM', font(44, bold=True), GOLD),
                        (1170, 'MemTotal: 43,164 KiB', font(29, mono=True), MUTED)],
                    top=859, height=72)
    elif kind == 'boot':
        wrap(d, 'The captured log identifies the script.', (65, 190), 1780, 60, bold=True)
        d.rounded_rectangle((65, 320, 1855, 625), radius=14, fill='#08131f', outline='#3a5366', width=2)
        lines = ['script=/mnt/sdcard/cubegm/icube.sh', 'event=start', 'event=launching /mnt/sdcard/cubegm/icube', 'event=started /mnt/sdcard/cubegm/icube child=556']
        for i, line in enumerate(lines):
            d.text((100, 355+i*58), line, font=font(31, mono=True), fill=GREEN if i==0 else INK)
        d.text((70, 659), 'EXCERPT FROM THE DEVICE BOOT TRACE', font=font(23, mono=True), fill=MUTED)
        wrap(d, 'Change cubegm/icube.sh → start Tiny MC directly', (70, 732), 1760, 43, GOLD, bold=True)
        wrap(d, 'The captured trace establishes this device’s observed cold-boot route.', (70, 835), 1760, 29, MUTED)
    elif kind == 'history':
        wrap(d, 'The work continued after the early videos.', (65, 185), 1760, 59, bold=True)
        rows = [('01 AUG', '28c46e23', 'VGA and VBE conformance notes'), ('05 SEP', 'f535f7fc', 'x86 source audit and regression probes'), ('27 SEP', '135b7201', 'Division overflow / guest fault handling'), ('27 SEP', 'd8292040', 'Overlapping MOVS regression matrix')]
        for i, (date, commit, text) in enumerate(rows):
            y = 342+i*126
            d.line((66, y+83, 1855, y+83), fill='#344859', width=2)
            aligned_row(d, [(70, date, font(27, mono=True), GREEN),
                            (310, commit, font(29, mono=True), GOLD),
                            (625, text, font(33, bold=True), INK)],
                        top=y-4, height=48)
        wrap(d, 'CODE AND HOST TEST RECORDS / separate from real-device validation', (70, 880), 1770, 27, MUTED, mono=True)
    elif kind in ['loader', 'driver']:
        title = 'The launcher becomes an entry point.' if kind == 'loader' else 'Reuse the vendor’s working hardware path.'
        wrap(d, title, (65, 190), 1770, 61, bold=True)
        blocks = [('Test ROM / file', 'Existing extension + per-file override'), ('rkgame', 'Core loading and libretro-style calls'), ('Custom MIPS core', 'My code draws frames and reads buttons')] if kind == 'loader' else [('Tiny MC / native app', 'A normal MIPS executable'), ('driver.so', 'Display, input and audio functions'), ('Device hardware', 'LCD / controls / sound')]
        for i, (heading, detail) in enumerate(blocks):
            y = 330+i*178
            d.rounded_rectangle((85, y, 1835, y+132), radius=14, fill='#203247', outline='#3a5366', width=2)
            aligned_row(d, [(120, heading, font(38, bold=True), GREEN),
                            (830, detail, font(29), INK)],
                        top=y, height=132)
            if i<2:
                d.text((920, y+133), '↓', font=font(35), fill=GOLD)
        wrap(d, scene['evidence'], (85, 905), 1750, 23, MUTED)
    else:
        wrap(d, scene['panel_title'], (65, 193), 1730, 29, GREEN, mono=True)
        y = wrap(d, scene['title'], (65, 260), 1700, 67, bold=True) + 40
        for i, point in enumerate(scene['points'], 1):
            y = numbered_point(d, i, point, number_x=80, text_x=170,
                               y=y, width=1630, size=39, number_size=29,
                               spacing=10) + 28
        assert y < 900, (scene['id'], 'Card overflow', y)
        wrap(d, 'RESEARCH NOTES / ' + scene['evidence'], (70, 910), 1760, 22, MUTED)
    im.save(output)


def spoken(text, story):
    for original, replacement in sorted(story['pronunciations'].items(), key=lambda x: -len(x[0])):
        text = text.replace(original, replacement)
    return text


def captions(scene, story, work, start):
    norm = lambda text: re.sub('[^a-z0-9]', '', text.lower())
    words = scene['narration'].split()
    canonical = ''.join(norm(spoken(word, story)) for word in words)
    spans, cursor = [], 0
    for line in (work/'voice'/(scene['id']+'.jsonl')).read_text(encoding='utf-8').splitlines():
        event = json.loads(line)
        token = norm(event['text'])
        position = canonical.find(token, cursor)
        if position<0:
            raise ValueError(('Caption alignment', scene['id'], event['text']))
        spans.append((position, position+len(token), event))
        cursor = position+len(token)
    aligned, cursor = [], 0
    for word in words:
        end = cursor+len(norm(spoken(word, story)))
        matching = [event for a,b,event in spans if b>cursor and a<end]
        if not matching:
            raise ValueError(('Missing word timing', scene['id'], word))
        aligned.append((word, matching[0]['offset']/1e7, (matching[-1]['offset']+matching[-1]['duration'])/1e7))
        cursor=end
    groups, group = [], []
    for word in aligned:
        # Keep reading speed and line length bounded, preserving the written script.
        if group and (len(' '.join(w[0] for w in group+[word]))>80 or word[2]-group[0][1]>5):
            groups.append(group);group=[]
        group.append(word)
        if re.search(r'[.!?]$', word[0]):
            groups.append(group);group=[]
    if group:
        groups.append(group)
    cues=[]
    for group in groups:
        cues.append([start+.65+group[0][1], start+.65+group[-1][2]+.1,
                     '\n'.join(textwrap.wrap(' '.join(w[0] for w in group), width=46))])
    for i in range(len(cues)-1):
        cues[i][1]=min(cues[i][1],cues[i+1][0]-.015)
    assert all(a<b for a,b,t in cues)
    return cues


def timestamp(value):
    milliseconds=round(value*1000)
    hours,milliseconds=divmod(milliseconds,3600000)
    minutes,milliseconds=divmod(milliseconds,60000)
    secs,milliseconds=divmod(milliseconds,1000)
    return f'{hours:02}:{minutes:02}:{secs:02},{milliseconds:03}'


def render(story, work, upload_dir=None):
    for folder in ['cards','clips','delivery']:
        (work/folder).mkdir(exist_ok=True,parents=True)
    hero=still(work/'sources/LYrICpUo6_A.mp4',55)
    source_hashes={s['id']:digest(work/'sources'/(s['id']+'.mp4')) for s in story['sources']}
    timeline,video_clips,audio_clips,all_cues=[],[],[],[]
    total_frames=0
    for index,scene in enumerate(story['scenes'],1):
        voice=work/'voice'/(scene['id']+'.mp3')
        frames=math.ceil((seconds(voice)+1.8)*FPS)
        duration=frames/FPS
        start=total_frames/FPS
        allocated=0
        weights=sum(s['weight'] for s in scene['shots'])
        shots=[]
        for shot_index,shot in enumerate(scene['shots']):
            count=round(frames*shot['weight']/weights) if shot_index<len(scene['shots'])-1 else frames-allocated
            allocated+=count
            length=count/FPS
            layout=work/'cards'/f'{index:02}-{shot_index:02}.png'
            dependencies=[]
            cmd=['ffmpeg','-hide_banner','-v','error','-y']
            if 'card' in shot:
                card(scene,shot['card'],index,hero,layout)
                cmd+=['-loop','1','-framerate',str(FPS),'-i',str(layout)]
                graph='[0:v]format=yuv420p,setsar=1[v]'
                dependencies=[digest(layout)]
            else:
                im,d=canvas(scene,index)
                sidebar(d,scene)
                d.rounded_rectangle((58,168,1342,892),radius=7,fill='#050b11',outline='#41586a',width=2)
                wrap(d,shot['label'],(64,916),1260,22,MUTED)
                im.save(layout)
                cmd+=['-loop','1','-framerate',str(FPS),'-i',str(layout)]
                if 'source' in shot:
                    path=work/'sources'/(shot['source']+'.mp4')
                    if shot['start']+length>seconds(path)-.05:
                        raise ValueError(('Shot exceeds source',scene['id'],shot))
                    cmd+=['-ss',str(shot['start']),'-threads','2','-reinit_filter','0','-i',str(path)]
                    dependencies=[source_hashes[shot['source']],digest(layout)]
                    scaler='lanczos'
                else:
                    path=REPO/shot['image']
                    if not path.exists():
                        path=HERE/'assets'/Path(shot['image']).name
                    cmd+=['-loop','1','-framerate',str(FPS),'-i',str(path)]
                    dependencies=[digest(path),digest(layout)]
                    scaler='neighbor'
                graph=f'[1:v]setpts=PTS-STARTPTS,fps={FPS},scale=1280:720:force_original_aspect_ratio=decrease:flags={scaler},pad=1280:720:(ow-iw)/2:(oh-ih)/2:color=0x050b11,setsar=1[clip];[0:v][clip]overlay=60:170:shortest=1,format=yuv420p[v]'
            fades=[]
            if shot_index==0:
                fades.append('fade=t=in:st=0:d=0.25')
            if shot_index==len(scene['shots'])-1:
                fades.append(f'fade=t=out:st={length-.3}:d=0.3')
            if fades:
                graph+=';[v]'+','.join(fades)+'[out]'
                mapping='[out]'
            else:
                mapping='[v]'
            output=work/'clips'/f'{index:02}-{shot_index:02}-{scene["id"]}.mp4'
            cmd+=['-filter_complex_threads','2','-filter_complex',graph,'-map',mapping,'-frames:v',str(count),'-an',
                  '-c:v','libx264','-preset','fast','-crf','20','-pix_fmt','yuv420p','-threads','4',
                  '-video_track_timescale','15360','-color_primaries','bt709','-color_trc','bt709','-colorspace','bt709',str(output)]
            signature=hashlib.sha256(json.dumps([cmd,dependencies]).encode()).hexdigest()
            cache=output.with_suffix('.sha256')
            if not (cache.exists() and output.exists() and cache.read_text()==signature):
                run(cmd,output.with_suffix('.log'))
                cache.write_text(signature)
            video=next(s for s in probe(output)['streams'] if s['codec_type']=='video')
            assert int(video['nb_frames'])==count,(output,'Rendered frame count')
            video_clips.append(output)
            shots.append(dict(**shot,frames=count,duration=length,timeline_start=start+(allocated-count)/FPS))
        soundtrack=work/'clips'/f'{index:02}-{scene["id"]}.wav'
        run(['ffmpeg','-v','error','-y','-i',str(voice),'-af',f'aresample=48000,adelay=650:all=1,apad,atrim=end_sample={frames*1600},asetpts=N/SR/TB',
             '-ar','48000','-ac','2','-c:a','pcm_s16le',str(soundtrack)])
        cues=captions(scene,story,work,start)
        assert cues[-1][1]<start+duration
        all_cues.extend(cues)
        audio_clips.append(soundtrack)
        timeline.append(dict(id=scene['id'],title=scene['title'],start=start,duration=duration,frames=frames,shots=shots))
        total_frames+=frames
        print(f'Rendered {index}/{len(story["scenes"])}: {scene["id"]} ({duration:.1f}s)',flush=True)
        (work/'timeline.json').write_text(json.dumps(timeline,indent=2)+'\n',encoding='utf-8')
    srt=work/'delivery'/(NAME+'.srt')
    srt.write_text('\n\n'.join(f'{i}\n{timestamp(a)} --> {timestamp(b)}\n{t}' for i,(a,b,t) in enumerate(all_cues,1))+'\n',encoding='utf-8')
    for name,paths in [('video-concat.txt',video_clips),('audio-concat.txt',audio_clips)]:
        (work/name).write_text(''.join("file '"+p.as_posix().replace("'","'\\''")+"'\n" for p in paths),encoding='utf-8')
    combined=work/'soundtrack.wav'
    run(['ffmpeg','-v','error','-y','-f','concat','-safe','0','-i',str(work/'audio-concat.txt'),'-c','copy',str(combined)])
    analysis=subprocess.run(['ffmpeg','-hide_banner','-i',str(combined),'-af','loudnorm=I=-16:TP=-1.5:LRA=11:print_format=json','-f','null','-'],capture_output=True).stderr.decode()
    measured=json.loads(analysis[analysis.rfind('{'):analysis.rfind('}')+1])
    (work/'loudness-input.json').write_text(json.dumps(measured,indent=2))
    normalizer='loudnorm=I=-16:TP=-1.5:LRA=11:linear=true:'+':'.join(f'{target}={measured[source]}' for target,source in [('measured_I','input_i'),('measured_TP','input_tp'),('measured_LRA','input_lra'),('measured_thresh','input_thresh'),('offset','target_offset')])
    normalized=work/'soundtrack-normalized.wav'
    run(['ffmpeg','-v','error','-y','-i',str(combined),'-af',normalizer,'-ar','48000','-c:a','pcm_s16le',str(normalized)])
    metadata=';FFMETADATA1\n'+f'title={story["title"]}\nartist={story["creator"]}\ncomment=Original project footage with synthetic English narration by Microsoft Jenny.\n'
    for scene in timeline:
        metadata+=f'\n[CHAPTER]\nTIMEBASE=1/1000\nSTART={round(scene["start"]*1000)}\nEND={round((scene["start"]+scene["duration"])*1000)}\ntitle={scene["title"]}\n'
    chapters=work/'chapters.ffmetadata'
    chapters.write_text(metadata,encoding='utf-8')
    output=work/'delivery'/(NAME+'.mp4')
    run(['ffmpeg','-hide_banner','-v','error','-y','-f','concat','-safe','0','-i',str(work/'video-concat.txt'),'-i',str(normalized),
         '-i',str(srt),'-i',str(chapters),'-map','0:v:0','-map','1:a:0','-map','2:0','-map_metadata','3','-map_chapters','3',
         '-c:v','copy','-c:a','aac','-b:a','192k','-ar','48000','-c:s','mov_text','-metadata:s:a:0','language=eng',
         '-metadata:s:s:0','language=eng','-metadata:s:s:0','title=English','-disposition:s:0','default','-movflags','+faststart',str(output)],work/'mux.log')
    (work/'delivery'/'English-narration.txt').write_text((HERE/'narration.en.txt').read_text(encoding='utf-8'),encoding='utf-8')
    source_report=[]
    for source in story['sources']:
        path=work/'sources'/(source['id']+'.mp4')
        source_report.append(dict(**source,sha256=source_hashes[source['id']],bytes=path.stat().st_size,duration=seconds(path)))
    (work/'delivery'/'sources.json').write_text(json.dumps(source_report,indent=2)+'\n',encoding='utf-8')
    (work/'delivery'/'timeline.json').write_text(json.dumps(timeline,indent=2)+'\n',encoding='utf-8')
    Image.open(work/'cards/01-02.png').save(work/'delivery/Cover.png')
    package_youtube(work, upload_dir)
    print(f'Finished: {output} / {total_frames/FPS:.3f} seconds / {len(all_cues)} captions',flush=True)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir',type=Path,default=DEFAULT_OUTPUT)
    parser.add_argument('--work-dir',type=Path)
    args=parser.parse_args()
    work=(args.work_dir or args.output_dir/'materials/work').resolve()
    render(json.loads((HERE/'story.json').read_text(encoding='utf-8')),work,args.output_dir)


if __name__=='__main__':
    main()

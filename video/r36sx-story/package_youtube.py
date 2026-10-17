"""Export the verified film's YouTube upload files without publishing it."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
NAME = 'R36SX-From-Stock-Firmware-to-DOS-English'
DEFAULT_OUTPUT = Path('C:/Work/YouTube Videos/R36SX')


def package(work, upload_dir=None):
    delivery = work / 'delivery'
    timeline = json.loads((delivery / 'timeline.json').read_text(encoding='utf-8'))
    metadata = json.loads((HERE / 'youtube.json').read_text(encoding='utf-8'))
    duration = sum(s['frames'] for s in timeline) / 30
    assert len(timeline) >= 3 and timeline[0]['start'] == 0
    assert all(s['duration'] >= 10 for s in timeline)
    chapters = '\n'.join(f'{int(s["start"])//60:02}:{int(s["start"])%60:02} '
                         + metadata['chapter_titles'][s['id']] for s in timeline)
    template = (HERE / metadata['description_template']).read_text(encoding='utf-8')
    description = template.format(chapters=chapters)
    assert len(metadata['title']) <= 100 and len(description.encode('utf-8')) <= 5000
    tags = ', '.join(metadata['tags'])
    tag_characters = sum(len(tag)+(2 if ' ' in tag else 0) for tag in metadata['tags']) + len(metadata['tags'])-1
    assert tag_characters <= 500
    outputs = {'YouTube-title-English.txt': metadata['title']+'\n',
               'YouTube-description-English.txt': description,
               'YouTube-tags-English.txt': tags+'\n'}
    for name, text in outputs.items():
        (delivery / name).write_text(text, encoding='utf-8')

    # Validate the separate caption file used by YouTube Studio.
    srt = delivery / (NAME + '.srt')
    cues = re.split(r'\n\s*\n', srt.read_text(encoding='utf-8').strip())
    previous_end = 0
    def seconds(stamp):
        h, m, s = stamp.replace(',', '.').split(':')
        return int(h)*3600+int(m)*60+float(s)
    for index, cue in enumerate(cues, 1):
        lines = cue.splitlines()
        assert int(lines[0]) == index and len(lines) >= 3
        a, b = map(seconds, lines[1].split(' --> '))
        assert previous_end <= a < b <= duration
        previous_end = b

    folder = Path(upload_dir).resolve() if upload_dir else delivery / 'YouTube-upload'
    folder.mkdir(exist_ok=True, parents=True)
    # Reuse the media export if present; do not recode the verified stream.
    for source, target in [(delivery / (NAME+'.mp4'), folder / (NAME+'.mp4')),
                           (srt, folder / (NAME+'.en.srt')),
                           (delivery / 'Cover.png', folder / 'Thumbnail.png')]:
        if not target.exists() or hashlib.sha256(source.read_bytes()).digest() != hashlib.sha256(target.read_bytes()).digest():
            shutil.copy2(source, target)
    for name in outputs:
        shutil.copy2(delivery / name, folder / name)
    shutil.copy2(delivery / 'English-narration.txt', folder / 'English-narration.txt')
    guide = '''YouTube upload package

1. Upload R36SX-From-Stock-Firmware-to-DOS-English.mp4.
2. Use YouTube-title-English.txt and YouTube-description-English.txt.
   The description contains chapters calculated from this exact final edit.
3. Set the video language to English. Add the English subtitles using
   Upload file > With timing and select the .en.srt file in this folder.
   The MP4 also contains an optional English subtitle track; upload the separate
   SRT in Studio so viewers can select captions there.
4. Use Thumbnail.png as the custom thumbnail. Tags are in YouTube-tags-English.txt.
5. Review YouTube's processing and checks, then choose visibility and publish.
   This package has not been uploaded or published; no YouTube checks have run.

Official references:
https://support.google.com/youtube/answer/2734796
https://support.google.com/youtube/answer/9884579
https://support.google.com/youtube/answer/72431
'''
    (folder / 'Upload-instructions.txt').write_text(guide, encoding='utf-8')
    report = dict(status='prepared_locally_not_uploaded', language=metadata['language'],
                  title=metadata['title'], duration_seconds=duration,
                  subtitle_cues=len(cues), chapters=len(timeline),
                  description_characters=len(description),
                  description_utf8_bytes=len(description.encode('utf-8')), tag_characters=tag_characters,
                  files={p.name:dict(bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())
                         for p in sorted(folder.iterdir()) if p.is_file() and p.name != 'package-manifest.json'})
    (folder / 'package-manifest.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({k:report[k] for k in ['status','duration_seconds','subtitle_cues','chapters','description_characters']}, indent=2))
    return folder


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work-dir', type=Path, default=DEFAULT_OUTPUT/'materials/work')
    parser.add_argument('--upload-dir', type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()
    package(args.work_dir.resolve(), args.upload_dir)

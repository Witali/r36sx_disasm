int check_encrypty(void)
{
    return 1;
}

int CheckEncrypty(const void *data)
{
    (void)data;
    return 1;
}

void exit(int status)
{
    (void)status;
    for (;;) {
        __asm__ volatile("");
    }
}

void abort(void)
{
    for (;;) {
        __asm__ volatile("");
    }
}

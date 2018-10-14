#include "mf_allpass.h"
#include "math.h"

mf_allpass *mf_allpass_new()
{
    mf_allpass *x = (mf_allpass *)malloc(sizeof(mf_allpass));
    x-> delay = 0;
    x-> counter = 1;
    x-> gain = .1;
    return x;
}

void mf_allpass_free(mf_allpass *x)
{
    free(x);
}

void mf_allpass_setDelay(mf_allpass *x, int delay)
{
    x->delay = delay;
}

void mf_allpass_perform(mf_allpass *x, float *in, float *out, int vectorSize)
{
    float delayout = 0;
    for (int i = 0; i < vectorSize; i++)
    {
        delayout = x->buffer[x->counter];
        x->buffer[(x->counter)] = in[i] + delayout * x->gain;
        out[i] = delayout - x->gain * in[i];
        
        if (x->counter != x->delay-1)
            x->counter++;
        else
            x->counter = 0;
    }
}

void mf_allpass_clearBuffer(mf_allpass *x)
{
    for (int i = 0; i < 10000; i++)
    {
        x->buffer[i] = 0;
    }
}

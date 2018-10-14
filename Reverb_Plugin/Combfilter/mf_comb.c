#include "mf_comb.h"
#include "math.h"

mf_comb *mf_comb_new()
{
    mf_comb *x = (mf_comb *)malloc(sizeof(mf_comb));
    x-> delay = 0;
    x-> counter = 1;
    x-> gain = 1;
    return x;
}

void mf_comb_free(mf_comb *x)
{
    free(x);
}

void mf_comb_setDelay(mf_comb *x, int delay)
{
    x->delay = delay/2;
}

void mf_comb_setGain(mf_comb *x, float t60, float fs)
{
    x->gain = pow(10,(-3*x->delay)/(t60*fs));
}

void mf_comb_perform(mf_comb *x, float *in, float *out, int vectorSize)
{
    float delayout = 0;
    for (int i = 0; i < vectorSize; i++)
    {
        delayout = x->buffer[x->counter];
        x->buffer[(x->counter)] = in[i] + (delayout * x->gain);
        out[i] = delayout;
        
        if (x->counter != x->delay-1)
            x->counter++;
        else
            x->counter = 0;
    }
}

void mf_comb_clearBuffer(mf_comb *x)
{
    for (int i = 0; i < 2000; i++)
    {
        x->buffer[i] = 0;
    }
}

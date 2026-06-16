
typedef struct {
    float kp, ki, hLim, lLim;
} PI_CTS;

void PI (PI_CTS gains, float *y, float x, float *y1, float *x1, float T);

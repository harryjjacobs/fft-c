#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "complex.h"

#define PI 3.14159265

#define assert_near(a, b) assert(fabs(a - b) < 1e-9)

double calc_amplitude(const Complex *complex, double n) {
  // the magnitude of the complex number, normalised by the number of sample
  // points
  double magnitude =
      sqrt(complex->real * complex->real + complex->imag * complex->imag);
  return magnitude / n;
}

double calc_phase(const Complex *complex) {
  return atan2(complex->imag, complex->real);
}

// https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm#Pseudocode
// Note to self as this seems to have just clicked in my head:
// The time-saving in the FFT comes from the fact that we only need to calculate
// DFT for N/2 each time, so at each recursion of the function we have a time
// saving of a half, because of the periodic nature of sin and cos. We know
// X_k+N/2 without iteration of k further because a jump by a multiple of PI
// means we are in the negative part of the "wave" that is symmetric to to the
// positive part in 0..N/2
void fft(Complex *in, Complex *out, unsigned int n, unsigned int stride) {
  printf("stride %d, n %d\n", stride, n);
  if (n == 1) {
    // at the lowest level division of N
    *out = *in;
    return;
  }
  // recursively split in two
  fft(in, out, n / 2, 2 * stride);                   // even: 0..N/2-1
  fft(in + stride, out + n / 2, n / 2, 2 * stride);  // odd: (N/2..N-1)
  // combine even and odd 'half' DFTs into the full DFT
  for (int k = 0; k < n / 2; k++) {
    double angle = -2.0 * PI * (double)k / n;
    Complex exp_factor = {.real = cos(angle),
                          .imag = sin(angle)};  // e^-(2pi/n)k
    Complex twiddle = complex_mult(exp_factor, out[k + n / 2]);
    // out[k] is the 'even' half, out[k + n / 2] is the 'odd' half
    out[k + n / 2] = complex_subtract(out[k], twiddle);
    out[k] = complex_add(out[k], twiddle);
  }
}

// frequency generator
double f(double t) {
  double hz_1 =
      2.0 * cos(1.0 * 2.0 * PI * t - PI);       // 1 Hz -180deg out of phase
  double hz_2 = 3.0 * cos(2.0 * 2.0 * PI * t);  // 2 Hz
  double hz_4 = 1.0 * cos(4.0 * 2.0 * PI * t);  // 4 Hz
  return hz_1 + hz_2 + hz_4;
}

void gen_input(Complex *input, int n, double period) {
  int i = 0;
  for (double t = 0.0; i < n; t += period, i++) {
    double signal = f(t);
    input[i].real = signal;
    input[i].imag = 0;
  }
}

int main(void) {
  const unsigned int N = 32;
  Complex in[N];
  Complex out[N];

  // generate input signal
  const double T = 0.1;  // sampling period T; 10 hz
  gen_input(in, N, T);

  fft(in, out, N, 1);

  // output
  printf("raw: \n");
  for (unsigned int i = 0; i < N; i++) {
    Complex c = out[i];
    printf("(%f,%f) ", c.real, c.imag);
  }
  printf("\n\n");
  printf("frequency,amplitude,phase: \n");
  // we use N / 2 as he spectrum of positive and negative frequencies are
  // symmetric for real inputs
  for (unsigned int k = 0; k < N / 2; k++) {
    Complex c = out[k];
    double f = (double)k / (N * T);  // the kth frequency
    double amplitude = calc_amplitude(&c, N);
    double phase = calc_phase(&c);
    printf("%f,%f,%f\n", f, amplitude, phase);
  }

  return 0;
}
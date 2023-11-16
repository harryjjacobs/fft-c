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

void dft(Complex *in, Complex *out, unsigned int n) {
  for (unsigned int k = 0; k < n; k++) {  // output element k
    out[k].real = 0.0;
    out[k].imag = 0.0;
    for (unsigned int j = 0; j < n; j++) {  // input element j
      // https://i.stack.imgur.com/j4l5B.png
      double angle = -2.0 * PI * k * (double)j / n;
      Complex e_term;
      e_term.real = cos(angle);
      e_term.imag = sin(angle);
      out[k] = complex_add(out[k], complex_mult(in[j], e_term));
    }
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
  // test cases
  {
    // test complex multiplication
    Complex a = {.real = 1, .imag = 2};
    Complex b = {.real = 0.2, .imag = -1.5};
    Complex c = complex_mult(a, b);
    assert_near(c.real, 3.2);
    assert_near(c.imag, -1.1);
  }
  {
    // test complex addition
    Complex a = {.real = 1, .imag = 2};
    Complex b = {.real = 0.2, .imag = -1.5};
    Complex c = complex_add(a, b);
    assert_near(c.real, 1.2);
  }

  const unsigned int N = 32;
  Complex in[N];
  Complex out[N];

  // generate input signal
  const double T = 0.1;  // sampling period T; 10 hz
  gen_input(in, N, T);

  dft(in, out, N);

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
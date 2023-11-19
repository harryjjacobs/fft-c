#ifndef H_COMPLEX
#define H_COMPLEX

// represents a complex number
typedef struct {
  double real;
  double imag;
} Complex;

Complex complex_mult(Complex lhs, Complex rhs) {
  Complex c;
  // i^2 = -1
  // a + bi
  c.real = lhs.real * rhs.real - lhs.imag * rhs.imag;
  c.imag = lhs.real * rhs.imag + rhs.real * lhs.imag;
  return c;
}

Complex complex_add(Complex lhs, Complex rhs) {
  Complex c;
  c.real = lhs.real + rhs.real;
  c.imag = lhs.imag + rhs.imag;
  return c;
}

Complex complex_subtract(Complex lhs, Complex rhs) {
  Complex c;
  c.real = lhs.real - rhs.real;
  c.imag = lhs.imag - rhs.imag;
  return c;
}

Complex complex_negate(Complex c) {
  Complex new_c = {.real = -c.real, .imag = -c.imag};
  return new_c;
}

#endif

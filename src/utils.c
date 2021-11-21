int debug_level = 1;

double
freq_to_period (double f, double samplerate)
{
  return f / samplerate;
}

void
phase_accu_update (double *phase_accu, double t)
{
  *phase_accu += t;
  if (*phase_accu > 1.0)
    {
      *phase_accu -= 1.0;
    }
}

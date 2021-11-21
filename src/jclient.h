#include "utils.h"

struct jclient
{

  jack_client_t *client;
  jack_port_t *output_port;
  jack_port_t *midi_input_port;
  jack_nframes_t bufsize;
  double samplerate;
  int xrun;
  double period;
  double volume;
  double phase_accu;
};

int jclient_run (struct jclient *);

void jclient_exit (struct jclient *);

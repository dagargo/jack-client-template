#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jack/intclient.h>
#include <jack/thread.h>
#include <jack/midiport.h>
#include <jack/jack.h>
#include "jclient.h"

static struct jclient jclient;

static int
jclient_thread_xrun_cb (void *cb_data)
{
  struct jclient *jclient = cb_data;
  error_print ("JACK xrun\n");
  jclient->xrun = 1;
  return 0;
}

static inline void
jclient_midi (struct jclient *jclient, jack_nframes_t nframes)
{
  jack_midi_event_t jevent;
  void *midi_port_buf;
  jack_nframes_t event_count;
  jack_midi_data_t status_byte;

  midi_port_buf = jack_port_get_buffer (jclient->midi_input_port, nframes);
  event_count = jack_midi_get_event_count (midi_port_buf);

  for (int i = 0; i < event_count; i++)
    {
      jack_midi_event_get (&jevent, midi_port_buf, i);
      status_byte = jevent.buffer[0];

      if (jevent.size == 1 && status_byte >= 0xf8 && status_byte <= 0xfc)
	{
	  //RT MIDI messages
	}
      else if (jevent.size == 2)
	{
	  switch (status_byte & 0xf0)
	    {
	    case 0xc0:		//Program Change
	      break;
	    case 0xd0:		//Channel Pressure (After-touch)
	      break;
	    }
	}
      else if (jevent.size == 3)
	{
	  switch (status_byte & 0xf0)
	    {
	    case 0x80:		//Note Off
	      break;
	    case 0x90:		//Note On
	      break;
	    case 0xa0:		//Polyphonic Key Pressure
	      break;
	    case 0xb0:		//Control Change
	      break;
	    case 0xe0:		//Pitch Bend Change
	      break;
	    }
	}
    }
}

static inline int
jclient_process_cb (jack_nframes_t nframes, void *arg)
{
  jack_default_audio_sample_t *buffer;
  struct jclient *jclient = arg;

  buffer = jack_port_get_buffer (jclient->output_port, nframes);

  for (int i = 0; i < nframes; i++)
    {
      buffer[i] = 0.0f;
    }

  jclient_midi (jclient, nframes);

  return 0;
}

void
jclient_exit (struct jclient *jclient)
{
}

int
jclient_run (struct jclient *jclient)
{
  jack_options_t options = JackNoStartServer;
  jack_status_t status;
  char *client_name;
  int ret = 0;

  jclient->client = jack_client_open ("jclient", options, &status, NULL);
  if (jclient->client == NULL)
    {
      error_print ("jack_client_open() failed, status = 0x%2.0x\n", status);

      if (status & JackServerFailed)
	{
	  error_print ("Unable to connect to JACK server\n");
	}

      ret = EXIT_FAILURE;
      return ret;
    }

  if (status & JackServerStarted)
    {
      debug_print (1, "JACK server started\n");
    }

  if (status & JackNameNotUnique)
    {
      client_name = jack_get_client_name (jclient->client);
      debug_print (0, "Name client in use. Using %s...\n", client_name);
    }

  if (jack_set_process_callback
      (jclient->client, jclient_process_cb, jclient))
    {
      ret = EXIT_FAILURE;
      goto cleanup_jack;
    }

  jclient->xrun = 0;
  if (jack_set_xrun_callback
      (jclient->client, jclient_thread_xrun_cb, jclient))
    {
      ret = EXIT_FAILURE;
      goto cleanup_jack;
    }

  jclient->samplerate = jack_get_sample_rate (jclient->client);
  printf ("JACK sample rate: %.0f\n", jclient->samplerate);

  jclient->bufsize = jack_get_buffer_size (jclient->client);
  printf ("JACK buffer size: %d\n", jclient->bufsize);

  jclient->output_port =
    jack_port_register (jclient->client,
			"output",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (jclient->output_port == NULL)
    {
      error_print ("Error while registering JACK audio out port\n");
      ret = EXIT_FAILURE;
      goto cleanup_jack;
    }

  jclient->midi_input_port =
    jack_port_register (jclient->client, "MIDI in", JACK_DEFAULT_MIDI_TYPE,
			JackPortIsInput, 0);

  if (jclient->midi_input_port == NULL)
    {
      error_print ("Error while registering JACK MIDI in port\n");
      ret = EXIT_FAILURE;
      goto cleanup_jack;
    }

  if (jack_activate (jclient->client))
    {
      error_print ("Cannot activate client\n");
      ret = EXIT_FAILURE;
      goto cleanup_jack;
    }

  while (1)
    {
      sleep (1);
    }

  debug_print (1, "Exiting...\n");
  jack_deactivate (jclient->client);

cleanup_jack:
  jack_client_close (jclient->client);

  return ret;
}

int
main (int argc, char *argv[])
{
  jclient_run (&jclient);
  return 0;
}

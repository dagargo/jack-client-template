# jack-client-template

This project is a template to create JACK clients.

## Installation

As with other autotools project, you need to run the following commands.

```
autoreconf --install
./configure
make
sudo make install
```

The package dependencies for Debian based distributions are:
- automake
- libtool
- jackd2
- libjack-jackd2-dev

You can easily install them by running `sudo apt install automake libtool jackd2 libjack-jackd2-dev`.

When installing `jackd2`, you would be asked to configure it to be run with real time priority. Be sure to answer yes. With this, the `audio` group would be able to run processes with real time priority. Be sure to be in the `audio` group too.

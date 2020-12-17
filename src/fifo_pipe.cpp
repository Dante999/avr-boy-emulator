#include "avr-boy-emulator/fifo_pipe.hpp"

#include <fcntl.h> // fifo
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h> // fifo write() read()

int fifo_pipe_c::open_pipe(mode_e rw_mode)
{
	umask(0);

	if ((mkfifo(m_filepath.c_str(), 0666)) == -1) {

		if (errno != EEXIST) {
			perror("mkfifo()");
			exit(EXIT_FAILURE);
		}
	}

	const int mode = (rw_mode == mode_e::READ ? O_RDONLY : O_WRONLY);

	/* FIFO zum Lesen und Schreiben öffnen */
	if ((m_fhandle = open(m_filepath.c_str(), mode)) == -1) {
		perror("open()");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int fifo_pipe_c::close_pipe()
{
	return close(m_fhandle);
}

void fifo_pipe_c::write_bytes(const uint8_t *data, size_t length)
{
	write(m_fhandle, data, length);
}

void fifo_pipe_c::read_bytes(uint8_t *data, size_t length)
{
	read(m_fhandle, data, length);
}
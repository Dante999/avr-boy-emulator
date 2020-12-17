#ifndef FIFO_PIPE_H
#define FIFO_PIPE_H

#include <string>

class fifo_pipe_c {

public:
	enum class mode_e { READ, WRITE };

private:
	const std::string m_filepath;
	const mode_e      m_mode;

	int m_fhandle;

public:
	fifo_pipe_c(const std::string &path, mode_e mode)
	    : m_filepath(path), m_mode(mode){};

	int  open_pipe();
	int  close_pipe();
	void write_bytes(const uint8_t *data, size_t length);
	void read_bytes(uint8_t *data, size_t length);
};
#endif /* FIFO_PIPE_H */

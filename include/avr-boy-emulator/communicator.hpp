#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <stdint.h>

class communicator_c {

public:
	enum class role_e : uint8_t { CARTRIDGE, HANDHELD };

private:
	const role_e m_role;

protected:
	virtual void    cartridge_init()                  = 0;
	virtual void    cartridge_send_byte(uint8_t byte) = 0;
	virtual uint8_t cartridge_receive_byte()          = 0;

	virtual void    handheld_init()                  = 0;
	virtual void    handheld_send_byte(uint8_t byte) = 0;
	virtual uint8_t handheld_receive_byte()          = 0;

public:
	communicator_c(role_e role) : m_role(role) {}

	virtual ~communicator_c() {}

	void init()
	{
		if (m_role == role_e::CARTRIDGE) {
			cartridge_init();
		}
		else {
			handheld_init();
		}
	}

	void send_byte(uint8_t byte)
	{
		if (m_role == role_e::CARTRIDGE) {
			cartridge_send_byte(byte);
		}
		else {
			handheld_send_byte(byte);
		}
	}

	uint8_t receive_byte()
	{
		if (m_role == role_e::CARTRIDGE) {
			return cartridge_receive_byte();
		}
		else {
			return handheld_receive_byte();
		}
	}
};

#endif /* COMMUNICATOR_H */

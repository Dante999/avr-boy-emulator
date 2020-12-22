#ifndef AVR_BOY_EMULATOR_H
#define AVR_BOY_EMULATOR_H

#include <memory>

#include <SFML/Graphics/Image.hpp>

#include "avr-boy-core/handheld.hpp"
#include "avr-boy-emulator/communicator.hpp"
#include "avr-boy-emulator/communicator_pipe.hpp"

class avr_boy_emulator_c {

private:
public:
	avr_boy_emulator_c(sf::Image *img);
	void do_work();
};
#endif /* AVR_BOY_EMULATOR_H */

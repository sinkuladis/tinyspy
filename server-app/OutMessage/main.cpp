#include "OutMessage.h"

int main(int argc, char **argv) {
    OutMessage message = OutMessage("Macius");
    std::cout << (char *)message.get_message() << std::endl;

    message.set_message("Ewelinka");
    std::cout << (char *)message.get_message() << std::endl;
}
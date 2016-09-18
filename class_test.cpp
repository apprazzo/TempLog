#include "CTCP_Client.h"
#include <string>

using namespace std;

int main() {
	CTCP_Client client;
	client.conn("pi", 1234);
	client.send_data("T = zu warm!");

	return 0;
}

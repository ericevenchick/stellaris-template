#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

void can_init() {
    unsigned long sysClk;

    // enable PORTE GPIO
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // set PE4 as CAN0RX in mux
    ROM_GPIOPinConfigure(GPIO_PE4_CAN0RX);
    // set PE5 as CAN0TX in mux
    ROM_GPIOPinConfigure(GPIO_PE5_CAN0TX);

    // set PE4 and PE5 to CAN type
    ROM_GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);

    // enable the CAN peripherial
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    // initialize the CAN module
    ROM_CANInit(CAN0_BASE);

    // set CAN bitrate
    ROM_CANBitRateSet(CAN0_BASE, ROM_SysCtlClockGet(), 500000);

    // leave init state, go to normal operation
    ROM_CANEnable(CAN0_BASE);
}

void can_tx() {
    tCANMsgObject txMsg;
    unsigned char data[] = {1,2,3,4,5,6,7,8};

    // populate the message
    txMsg.ulMsgID = 0;
    txMsg.ulFlags = 0;
    txMsg.ulMsgLen = 8;
    txMsg.pucMsgData = data;

    // send the message
    ROM_CANMessageSet(CAN0_BASE, 1, &txMsg, MSG_OBJ_TYPE_TX);
}

int main()
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN);

    can_init();

    for (;;) {
        can_tx();
        // set the red LED pin high, others low
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED |
                         LED_BLUE | LED_GREEN);
        ROM_SysCtlDelay(5000000);
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, 0);
        ROM_SysCtlDelay(5000000);
    }

}

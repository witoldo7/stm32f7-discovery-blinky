
#include "hw_config.h"

int main(void)
{
	/* Set Basis System(includes SYSTICK Setings) */
	Set_System();

    /* Set UART and redirect to stdio */
    conio_init(UART_DEFAULT_NUM,UART_BAUDRATE);

    BSP_LED_Init(LED_GREEN);


	int i = 0;

    for(;;)
    {
        BSP_LED_Toggle(LED_GREEN);

        for (int j = 0; j < 1000000; j++)
            ;

        printf("Hello, world! -- %i\n", i++);
    }

    return 0;
}

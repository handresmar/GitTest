#include "soc-hw.h"

uart_t  *uart0  = (uart_t *)   0x20000000;
timer_t *timer0 = (timer_t *)  0x30000000;
pwm_t   *pwm0   = (pwm_t *)    0x40000000;
gpio_t  *gpio0  = (gpio_t *)   0x40000000;
uart_t  *uart1  = (uart_t *)   0x50000000;
i2c_t   *i2c0   = (i2c_t *)    0x60000000;

isr_ptr_t isr_table[32];

void prueba()
{
	uart0->rxtx=30;
	uart1->rxtx=30;
	timer0->tcr0 = 0xAA;
	//gpio0->ctrl=0x55;
	//spi0->rxtx=1;
	//spi0->nop1=2;
	//spi0->cs=3;
	//spi0->nop2=5;
	//spi0->divisor=4;
	i2c0->rxtx=5;
	i2c0->divisor=5;

}
void tic_isr();
/***************************************************************************
 * IRQ handling
 */
void isr_null()
{
}

void irq_handler(uint32_t pending)
{
	int i;

	for(i=0; i<32; i++) {
		if (pending & 0x01) (*isr_table[i])();
		pending >>= 1;
	}
}

void isr_init()
{
	int i;
	for(i=0; i<32; i++)
		isr_table[i] = &isr_null;
}

void isr_register(int irq, isr_ptr_t isr)
{
	isr_table[irq] = isr;
}

void isr_unregister(int irq)
{
	isr_table[irq] = &isr_null;
}

/***************************************************************************
 * TIMER Functions
 */
void msleep(uint32_t msec)
{
	uint32_t tcr;

	// Use timer0.1
	timer0->compare1 = (FCPU/1000)*msec;
	timer0->counter1 = 0;
	timer0->tcr1 = TIMER_EN;

	do {
		//halt();
		tcr = timer0->tcr1;
	} while ( ! (tcr & TIMER_TRIG) );
}

void nsleep(uint32_t nsec)
{
	uint32_t tcr;

	// Use timer0.1
	timer0->compare1 = (FCPU/1000000)*nsec;
	timer0->counter1 = 0;
	timer0->tcr1 = TIMER_EN;

	do {
		//halt();
		tcr = timer0->tcr1;
	} while ( ! (tcr & TIMER_TRIG) );
}


uint32_t tic_msec;

void tic_isr()
{
	tic_msec++;
	timer0->tcr0     = TIMER_EN | TIMER_AR | TIMER_IRQEN;
}

void tic_init()
{
	tic_msec = 0;

	// Setup timer0.0
	timer0->compare0 = (FCPU/10000);
	timer0->counter0 = 0;
	timer0->tcr0     = TIMER_EN | TIMER_AR | TIMER_IRQEN;

	isr_register(1, &tic_isr);
}



/***************************************************************************
 * WIFI ESP8266 Functions
 */

void init_wifi(){ //configurar el modulo como estación con puerto 80
	uart_putstr("AT+RST\r\n");
	msleep(200);
	int c = 0;
	while(c==0){
		uart_putstr("AT+CIPMUX=1\r\n");
		c = ok();
	}
	c = 0;
	while (c==0){
		uart_putstr("AT+CIPSERVER=1,80\r\n");
		c = ok();
	}
}

void wifi_putchar(char a){
	int c = 0; 	  
	while(c == 0){
		uart_putstr("AT+CIPSEND=0,1\r\n");
		uart_putchar(a);
		c = ok();
	}
}

char wifi_getchar(){
	char c='\n';
	int i=0;
	for(i=0;i<20;i++){
		c = uart_getchar();
		if (c ==':'){
			c = uart_getchar();
			return c;
			break;
		}
	}
	return '\n';

}

int ok(){
	int i=0;
	char a;
	for(i=0;i<30;i++){
		a=uart_getchar();
		if(a=='K'){
			return 1;
		}
	}
	return 0;

}

/***************************************************************************
 * UART Functions
 */
void uart_init()
{
	//uart0->ier = 0x00;  // Interrupt Enable Register
	//uart0->lcr = 0x03;  // Line Control Register:    8N1
	//uart0->mcr = 0x00;  // Modem Control Register

	// Setup Divisor register (Fclk / Baud)
	//uart0->div = (FCPU/(57600*16));
}

char uart_getchar()
{   
	while (! (uart0->ucr & UART_DR)) ;
	return uart0->rxtx;
}

void uart_putchar(char c)
{
	while (uart0->ucr & UART_BUSY) ;
	uart0->rxtx = c;
}

void uart_putstr(char *str)
{
	char *c = str;
	while(*c) {
		uart_putchar(*c);
		c++;
	}
}


//uart1

char uart_getchar1()
{   
	while (! (uart1->ucr & UART_DR)) ;
	return uart1->rxtx;
}

void uart_putchar1(char c)
{
	while (uart1->ucr & UART_BUSY) ;
	uart1->rxtx = c;
}

void uart_putstr1(char *str)
{
	char *c = str;
	while(*c) {
		uart_putchar1(*c);
		c++;
	}
}

/***************************************************************************
 * Functions PWM //FUNCIONES PRIMITIVAS
 */

void set_period(int p){
	pwm0->period=p;
}
void set_motor1(int wk){
	pwm0-> duty_m1=wk;
}
void set_motor2(int wk){
	pwm0-> duty_m2=wk;
}
void set_motor3(int wk){
	pwm0-> duty_m3=wk;
}
void set_motor4(int wk){
	pwm0-> duty_m4=wk;
}


/***************************************************************************
 * Functions motors //FUNCIONES PRIMITIVAS
 */

void setMotors(){
	set_period(0x1E8480); //F=50Hz
	set_motor1(0xEA60);
	set_motor2(0xEA60);
	set_motor3(0xEA60);
	set_motor4(0xEA60);
	msleep(0x3E8);
}

void up_down_q(int s){
	int vel;
	int velM3;
	if(s>=0x1){
		vel = 0x5DC * s + 0xFD20;
		
	}
	else{
		vel= 0xEA60;
		velM3 = 0xEA60; 
	}
	set_motor1(vel);
	set_motor2(vel);
	set_motor3(vel);
	set_motor4(vel);
}







/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

/* STEP 9 - Increase the sleep time from 100ms to 10 minutes  */
#define SLEEP_TIME_MS   100*60*1000

/* SW0_NODE is the devicetree node identifier for the "sw0" alias */
#define SW0_NODE	DT_ALIAS(sw0) 
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

/* LED0_NODE is the devicetree node identifier for the "led0" alias. */
#define LED0_NODE	DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);


/* STEP 4 - Define the callback function */
void button_pressed(const struct device *dev, struct gpio_callback *cd, uint32_t pins){
	//QUES: What is a callback function? 
	//ANS:  This function is called whenever interrupt is activated
	//QUES: why do I passed all 3 different paramenter to this function?
	//		Can I just pass the structure, and not the rest?
	//ANS:  ************
	gpio_pin_toggle_dt(&led);
	//Passing in address of the struct led
}

/* STEP 5 - Define a variable of type static struct gpio_callback */
//QUES: Why do we need this struct? 
//		What is button_cb_data?
//ANS:  This statement is just a define statement.
//		This is to allocate the right budget for the memeory.
//		No value is added in this struct. Just an empty sturct.
//		Idk why can't we just define everything up here instead of down there.
static struct gpio_callback button_cb_data;



int main(void)
{
	int ret;

	if (!device_is_ready(led.port)) {
		return -1;
	}

	if (!device_is_ready(button.port)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}

	/* STEP 3 - Configure the interrupt on the button's pin */
	ret =  gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	
	/* STEP 6 - Initialize the static struct gpio_callback variable   */
	//QUES: What is gpio_init_callback function?
	//		What parameters does this function received?
	//ANS:	The parameters are:
	//			- Address of the previously define gpio_callback struct
	//			- The  previously callback function (what the MC should do when interrupt is detected) 
	//			- The BIT function, use to create bit mask for the value of: button.pin
	//			  ex: if button.pin = 10, then the result is 1 << 10 is 1024
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));


	/* STEP 7 - Add the callback function by calling gpio_add_callback()   */
	//The parameters are: 
	//	- Port element in button struct
	//	- Address of the callback struct 
	gpio_add_callback(button.port, &button_cb_data);

	while (1) {
		/* STEP 8 - Remove the polling code */	
		//Sleep time can be as long as 10 minutes and device would work just fine.
        k_msleep(SLEEP_TIME_MS); 

	}
        
}
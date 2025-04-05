#include <video.h>
#include <draw.h>
#include "ps2_helper.h"
#include "ps2mouse.h"
#include "cursor.h"

int mCoordx = 500;
int mCoordy = 500;
int lmCoordx = 500;
int lmCoordy = 500;

size_t mouse_cycle_count = 0;
// 3 bytes are sent each mouse cycle
mouse_data_t mouse_data;

bool mouse_packet_ready = false;

/* Explanation: mouse_recv() is called by IRQ 12 3 times every time the mouse is moved, sending a byte each time. As such, we must handle each case seperately
   Full credit to PonchoOS on youtube (https://www.youtube.com/watch?v=Ix5-ixq6aSk&list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ&index=14&ab_channel=Poncho) I used his 
   tutorial to figure out how the hell this works, I was stuck on this for a while without it so thank you :) */
void mouse_recv() {
    /* 
       This changes what is done depending on which byte we are on; if this is the first time in this "mouse cycle" 0 will be run, if its the 1st time then 1 will be called
       etc.
    */
    switch(mouse_cycle_count) {
    case 0:
        mouse_data.status_byte = inb(0x60);
        // if we are already processing a packet, ignore this packet
       // if (mouse_packet_ready) break;
        // the byte is corrupted, the always one flag should be set
        if (mouse_data.status_byte & PS2_ALWAYS_ONE_FLAG == 0) break;
        mouse_cycle_count++;
        break;
    case 1:
        mouse_data.x_movement = inb(0x60);
        // if we are already processing a packet, ignore this packet
       // if (mouse_packet_ready) break;
        mouse_cycle_count++;
        break;
    case 2:
        mouse_data.y_movement = inb(0x60);
        /* 
           Since this function is called asynchronously (through interupts) its possible we could accidentally handle two packets simulatneously (sort of, still single threaded).
           this will cause issues so mouse_packet_ready functions as "lock" feature
        */
      //  if (mouse_packet_ready) break;
     //   else mouse_packet_ready = true;
        mouse_cycle_count = 0;
        int xMov = 0;
        int yMov = 0;

        // Now that we have reached the end of the cycle, calculate everything and update the mouse values

        // In the case of overflow, just set the max 8 bit value as the movement has gone too fast to properly be set in 8 bits 
        if (mouse_data.status_byte & (PS2_X_OVERFLOW_BIT)) xMov = 255;
        if (mouse_data.status_byte & (PS2_Y_OVERFLOW_BIT)) yMov = 255;


        // if the x sign bit is set, the x movement should be negative
        if (mouse_data.status_byte & (PS2_X_SIGN_BIT)) {
            // some 8-bit overflow magic, not too sure!
            mouse_data.x_movement = 256 - mouse_data.x_movement;
            xMov = -((int)mouse_data.x_movement);
        } else {
            xMov = ((int)mouse_data.x_movement);
        }

        // if the y sign bit is set, the y movement should be negative
        if (mouse_data.status_byte & (PS2_Y_SIGN_BIT)) {
            // some 8-bit overflow magic, not too sure!
            mouse_data.y_movement = 256 - mouse_data.y_movement;
            yMov = -((int)mouse_data.y_movement);
        } else {
            yMov = ((int)mouse_data.y_movement);
        }

        // make sure the mouse is in bounds before updating the mouse coordinates
        if (mCoordx + xMov >= SCREEN_WIDTH || mCoordx + xMov <= 0) {}
        else
            mCoordx += xMov;
        if (mCoordy - yMov >= SCREEN_HEIGHT || mCoordy - yMov <= 0) {}
        else
            mCoordy -= yMov;
        
        mouse_packet_ready = false;
        break;
    }
}


int init_ps2_mouse() {
    // Set the deafult settings of the mouse
    ps2_controller_waittowrite();
    write_to_mouse(PS2_DEVICE_SET_DEFAULTS);
    ps2_controller_waittoread();  
    byte c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;

    // Enable keyboard scanning
    ps2_controller_waittowrite();
    write_to_mouse(PS2_DEVICE_ENABLE_SCANNING);
    ps2_controller_waittoread();  
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;

    const int sample_rate = 200;
    // Enable keyboard scanning
    ps2_controller_waittowrite();
    write_to_mouse(PS2_MOUSE_SET_SAMPLE_RATE);
    ps2_controller_waittoread();  
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;
    ps2_controller_waittowrite();
    write_to_mouse(sample_rate);
    ps2_controller_waittoread();  
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;

    
    return SUCCESS;   
}
extern int sec;
void restart_ps2_mouse() {
    // Disable keyboard scanning
    ps2_controller_waittowrite();
    write_to_mouse(PS2_DEVICE_DISABLE_SCANNING);
    ps2_controller_waittoread(); 
    byte c = recv_from_mouse();
    if (c != PS2_ACK)
        return;

    ps2_controller_waittowrite();
    write_to_mouse(PS2_RESET_DEVICE);
    ps2_controller_waittoread();
    print_hex(recv_from_mouse());
    swap();

    // Disable keyboard scanning
    ps2_controller_waittowrite();
    write_to_mouse(PS2_DEVICE_ENABLE_SCANNING);
    ps2_controller_waittoread(); 
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return;

    init_ps2_mouse();
}



void draw_cursor() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (cursor[j+i*20])
                plot_pixel(i+mCoordx, j+mCoordy, rgb(0, 0, 0));
        }
    }  
}
#include <pit.h>
#include <video.h>
#include <minecraftOS.h>
#include "screensaver.h"
#include "screensavers/screensaver1.h"
#include "screensavers/screensaver2.h"
#include "screensavers/screensaver3.h"
#include "screensavers/screensaver4.h"
#include "screensavers/screensaver5.h"
#include "screensavers/screensaver6.h"


size_t start_seconds;

typedef struct {
    const uint32_t* p_img_data;
    size_t width;
    size_t height;
} screensaver_data;

// Do not put this in screensaver.h, we want to keep #include "screensavers/screensaverX.h" in this file only because compiling all images each time is stupid
const screensaver_data p_screensavers[] = { {SCREENSAVER1_data, SCREENSAVER1_WIDTH, SCREENSAVER1_HEIGHT}, 
                                            {SCREENSAVER2_data, SCREENSAVER2_WIDTH, SCREENSAVER2_HEIGHT}, 
                                            {SCREENSAVER3_data, SCREENSAVER3_WIDTH, SCREENSAVER3_HEIGHT}, 
                                            {SCREENSAVER4_data, SCREENSAVER4_WIDTH, SCREENSAVER4_HEIGHT}, 
                                            {SCREENSAVER5_data, SCREENSAVER5_WIDTH, SCREENSAVER5_HEIGHT}, 
                                            {SCREENSAVER6_data, SCREENSAVER6_WIDTH, SCREENSAVER6_HEIGHT}
                                        };

constexpr const size_t num_screensavers = sizeof(p_screensavers) / sizeof(p_screensavers[0]);

bool in_screensaver = false;
void screensaver_start() {
    in_screensaver = true;
    start_seconds = seconds_passed;
    while (in_screensaver) {
        for (int i = 0; i < num_screensavers; i++) {
            draw_image(p_screensavers[i].p_img_data, p_screensavers[i].width, p_screensavers[i].height);
            swap(); 
            sleep_seconds(SCREENSAVER_SHOW_TIME_SECONDS);
        }
    }

}

void screensaver_stop() {
    in_screensaver = false;
}
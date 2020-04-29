# Olympic Cycling Bot
Programmed Parallax BOEBot with Arduino to compete in an Olympic cycling event as part of a team of bots.
A demonstration can be seen [here](https://www.youtube.com/watch?v=sCExtE_GScI&list=PLDTd6Y9DIjMjLb99Wq5d-gKtAXU-chyQ8&index=3).

## Overview
The theme of the team challenge was the BOE-lympics, taking place in BOE-lympic Stadium, a table top decorated with the Olympic rings. The team challenge involved integrating five BOE-bots to parade around the table, judge a series of sporting events (diving, weightlifting, cycling, basketball, and court sports), and to award a gold, silver, or bronze medal for each event based on the object detected within the ring. The bots then generated a collective, integrated result to perform the associated celebration.

<p align="center">
  <img width="400" height="500" src="/Figures/FrontView.jpg">
</p>

## Cycling Bot Description
My bot judged the cycling event, where medals were awarded based on the slope of the given incline. The bot first circumnavigated its ring and then correctly determined the grade of ramp placed in the middle of the ring, corresponding to the event the competitor was racing. Zero grade equated to the track event for the bronze medal, the moderate grade matched to the road race for the silver medal, and the steep grade corresponded to the mountain for the gold.

### Objectives
* Line following: 
   + via decay times from QTI sensors to differentiate black line from white surface
* Sense ramp steepness
   + accelerometer and three thresholds (no ramp, medium ramp, large ramp)
* Inter-bot communication
   + via X-Bee module, while score display occurred via LCD screen
* Celebration action based on team scored received
   + national anthem, light show, and dance
  

### Design
In order to complete the above tasks, the bot was equipped with wheels, an X-bee, QTI sensors, an accelerometer,
a piezospeaker, an LCD screen, and LEDs. Wiring and circuit diagrams can be viewed in `/Figures`. Four QTI sensors, detecting color via IR reflection, were used for
the line following task. Object sensing was accomplished with the Memsic 2125 Dual-Axis Accelerometer, a sensor
able to measure tilt in the x- and y-directions. A back red LED (on the back breadboard) was installed to display this detection. A score of 2 corresponded to two blinks, a score of 1 corresponded to one blink, and a score of 0 corresponded to no blinks. Finally, once the score was obtained, the value was displayed on an LCD screen by the judge. Celebrations were then performed depending on overall team success; the national anthem was performed with the piezospeaker built in to the LCD screen, the light show was performed with addition green and yellow LEDs, and the dance was performed using the servos.

### Results
The results demonstrated that the QTI sensors could provide distinct values based on the color of the surface, thereby allowing the designed bot to trace the ring surrounding the vent. The bot could receive and transmit information using the X-bee unit. It was able to obtain distinct accelerometer values based on tilt of the bot, hence allowing the differentiation among the three different slopes. The team trials also demonstrated that the bot was capable of performing the celebration, may it be dancing, a light show, or the national anthem.

## Bot Collaboration
For each event, a bronze medal resulted in a score of 0, the silver medal in a score of 1, and the gold medal in a score of 2. After determining the medal won at the individual event, the judge displayed the resulting score via LEDs. Finally, the team of judges communicated the results of their respective events to a designated head judge (`SentryBot`) to come to a consensus to award an overall medal to the competing nation and perform a celebration act accordingly. The head judge summed the scores, took the modulus 3, and converted the result to binary. This result was then communicated to all other bots for display via LEDs or an LCD screen. If the result was 10, the nation received a gold medal and the judges sang the national anthem. If the result was 01, a silver medal was awarded and the judges performed a light show. If the result was 00, the nation won the bronze medal and the judges performed a dance.

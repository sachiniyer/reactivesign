# Customizable LED sign

This is a customizable LED sign, which you can connect to AWS IOT to change colors based on a website. This can also serve as a model to for most IOT applications with esp8266.

## Features

MQTT connection with AWS IOT
Carries color values over power cycles through virtual EEPROM
Accounts for occasional reboots through EEPROM
Automatic reconnection to AWS IOT

## Parts List

LED strip (I used [this](https://www.amazon.com/BTF-Lighting-Individually-Addressable-Flexible-Lighting/dp/B075T7GQRP?pd_rd_w=l56PO&pf_rd_p=7a695a6b-3610-4583-9730-4d63d22b28b7&pf_rd_r=T0101DF08KD709APP1ZP&pd_rd_r=44d72d7c-5c85-461d-a2ee-c6e673e116b3&pd_rd_wg=QhGvn&pd_rd_i=B075T7GQRP&psc=1&ref_=pd_bap_d_rp_12_t))

Around 120Ohm resistor between LED strip and ESP8266

[ESP8266](https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B081CSJV2V/ref=sr_1_1_sspa?dchild=1&keywords=esp8266&qid=1613470764&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyMDU1Wk1ETFlaWjBVJmVuY3J5cHRlZElkPUEwNjMxMjQxMVdNMUEzQldKRkhIMCZlbmNyeXB0ZWRBZElkPUEwNTYyNjkzMU5WTEI1SjdJUTlDJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)

A micro usb cable

## Hardware stuff
1. Connect the led strip's data cable (green) to pin 2 of the esp8266.
2. Connect the led strip's power cables directly to the power cables of the usb cable
3. Insert the microusb cable into the esp8266. You may want to use a separate cable for uploading code, and just use this cable during production.

## AWS stuff

### API gateway
I created an API gateway, which then connects through proxy to a lambda function. This is running through my personal website, which is a REST API. I created a separate endpoint for the lambda execution, but you don't have to.

### Lambda function
The lambda function then just transfers the payload straight through AWS IOT with an out topic. The topic is called inTopic, because I'm lazy. I included the lambda function code as well in "./aws/" (also I'm trying to use more javascript in my life, so it is a bit badly written). The payload is hex values, because I did not want to send actual json. The hex values are converted in RGB on the device itself.

### IOT Stuff
Set up a generic iot device and download the certs that you automatically generate. Then you can just copy and paste those certs into the main sketch where it says to do so. You may want to do some of your own tests to make sure a connection is established before you start with the actual stuff (there is a sketch to test under "./aws/testconnection.ino").

### Hosting yourself
It would also be very easy to host this yourself. I chose AWS, because I had an API gateway running and it would be easy. However, modifying this to send MQTT data over your own server/authenticating with your own server is fairly trivial.

## Website stuff
Just change your API gateway endpoint to your own endpoint. Easy stuff. Under "./website/". Mine is literally just two color pickers, but you may want to add some css and stuff to make it look more pretty.

## Generating name values
You are going to want to create a txt file with an approximation of which lights are reserved for the name, and which lights are reserved for the background, then using the script "findout.py", you should be able to get the outputs (This is specified for the led strip I included, you will have to modify it for a different script). An example is included under "name.txt".

## Final notes
This was meant to be a really quick project to the code is nice and dirty, as with a lot of the other things I do. Some things to note, are that on initial upload there may be some problems with the color values, because it reads garbage input from the flash. Hopefully it will just default to red and blue though. Also, if someone actually thinks about building this, send me a message, I'd love to help with any questions. [This](https://dxuhpsew7hntg.cloudfront.net/sign_display.mp4) is an example when I was testing.

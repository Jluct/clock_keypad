#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

unsigned long millist = 0;
int hour = 0;
int minute = 0;
int second = 0;
int day_ard = 1;
boolean menu_b = false;

void setup() {
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
}

void loop() {
    menu();
    lcd.setCursor(0, 0);

    String day_name = day();
    lcd.print(day_name);

    String day_inf = info_day();
    if (day_inf.length() + day_name.length() >= 15) {
        lcd.setCursor(3, 0);
        lcd.print(".     ");
    }
    lcd.print("     ");
    lcd.setCursor(16 - day_inf.length(), 0);
    lcd.print(day_inf);


    lcd.setCursor(0, 1);
    inc_time();
    lcd.print("Time:   ");
    lcd.print(time());

    delay(200);
}

void inc_time() {
    if (millis() < millist) millist = millis() + 200;
    int plus = (millis() - millist) / 1000;
    millist += plus * 1000;

    second += plus;

    time_control();
}

void time_control() {
    if (second >= 60) {
        second -= 60;
        minute++;

        if (minute >= 60) {
            minute -= 60;
            hour++;

            if (hour >= 24) {
                hour -= 24;
                day_ard++;

                if (day_ard > 7) day_ard -= 7;
            }
        }
    }
}

String time() {
    String result = "";
    if (hour < 10) result += '0';
    result += String(hour) + ':';
    if (minute < 10) result += '0';
    result += String(minute) + ':';
    if (second < 10) result += '0';
    result += String(second);

    return result;
}

String day() {
    String result;

    switch (day_ard) {
        case 1:
            result = "Monday";
            break;
        case 2:
            result = "Tuesday";
            break;
        case 3:
            result = "Wednesday";
            break;
        case 4:
            result = "Thursday";
            break;
        case 5:
            result = "Friday";
            break;
        case 6:
            result = "Saturday";
            break;
        case 7:
            result = "Sunday";
            break;
    }

    return result;
}

String info_day() {
    String result;

    if (hour >= 5 && hour < 12) {
        result = "morning";
    } else if (hour >= 12 && hour < 13) {
        result = "noon";
    } else if (hour >= 13 && hour < 18) {
        result = "afternoon";
    } else if (hour >= 18 && hour < 22) {
        result = "evening";
    } else if (hour >= 22 && hour < 24) {
        result = "night";
    } else if (hour == 0) {
        result = "midnight";
    } else if (hour >= 1 && hour < 5) {
        result = "late night";
    }

    return result;
}

int read_LCD_buttons() {
    int adc_key_in = analogRead(0);
    if (adc_key_in > 1000) return btnNONE;
    if (adc_key_in < 50) return btnRIGHT;
    if (adc_key_in < 200) return btnUP;
    if (adc_key_in < 400) return btnDOWN;
    if (adc_key_in < 600) return btnLEFT;
    if (adc_key_in < 800) return btnSELECT;

    return btnNONE;
}

boolean settings() {
    static int str_menu = 0; // 3 = day, 2 = hour, 1 =  minute, 0 = second
    int lcd_key = read_LCD_buttons();

    lcd.setCursor(0, 0);
    lcd.setCursor(0, 1);
    lcd.print(day());
    lcd.setCursor(3, 1);
    lcd.print(".    ");
    lcd.print(time());

    while (menu_b && lcd_key == btnNONE) {
        lcd_key = read_LCD_buttons();
    }

    switch (lcd_key) {
        case btnRIGHT: {
            if (str_menu > 0) str_menu--;
            break;
        }
        case btnLEFT: {
            if (str_menu < 3) str_menu++;
            break;
        }
        case btnUP: {
            switch (str_menu) {
                case 0:
                    second++;
                    break;
                case 1:
                    minute++;
                    break;
                case 2:
                    hour++;
                    break;
                case 3:
                    day_ard++;
                    break;
            }

            break;
        }
        case btnDOWN: {
            switch (str_menu) {
                case 0:
                    second--;
                    break;
                case 1:
                    minute--;
                    break;
                case 2:
                    hour--;
                    break;
                case 3:
                    day_ard--;
                    break;
            }

            break;
        }
        case btnSELECT: {
            menu_b = !menu_b;

            if (menu_b) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("SETTINGS  ");
                settings();
            } else {
                delay(200);
                return false;
            }

            break;
        }
    }

    time_control();

    delay(200);
    return true;
}

void menu() {
    if (!menu_b) {
        if (read_LCD_buttons() == btnSELECT) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("SETTINGS  ");
            menu_b = !menu_b;
            delay(200);
        } else {
            return;
        }
    }

    while (settings());
    millist = millis();
}

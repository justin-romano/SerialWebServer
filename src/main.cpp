#include <Arduino.h>
#include <ArduinoJson.h>
void setup()
{
    Serial.begin(115200);
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A0, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(7, OUTPUT);
}
StaticJsonDocument<512> doc;
template <class T>
void serializeJsonLine(T doc)
{
    while (!Serial.availableForWrite())
    {
    };
    doc["Data"]["TE"] = JsonInteger(millis());
    serializeJson(doc, Serial);
    Serial.write("\n");
    Serial.flush();
}

void loop()
{
    while (!Serial.available())
    {
    }
    auto red = Serial.readStringUntil('\n');
    auto error = deserializeJson(doc, red);
    if (error)
    {
        Serial.write(String("error " + String((int)error.code()) + " ").c_str());
        Serial.write(String(red + "\n").c_str());
        //Serial.flush();
    }
    doc["Data"] = JsonObject();
    doc["Data"]["TS"] = JsonInteger(millis());
    if (doc["Url"] == "/turnOn")
    {
        digitalWrite(7, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        doc["Data"]["Message"] = "relay on sucucess";
        serializeJsonLine(doc);
    }
    else if (doc["Url"] == "/turnOff")
    {
        digitalWrite(7, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        doc["Data"]["Message"] = "relay off sucucess";
        serializeJsonLine(doc);
    }
    else if (doc["Url"] == "/getArduinoInfo")
    {
        doc["Data"]["LL"] = JsonInteger(analogRead(A0));
        doc["Data"]["M1"] = JsonInteger(analogRead(A1));
        doc["Data"]["M2"] = JsonInteger(analogRead(A2));
        doc["Data"]["M3"] = JsonInteger(analogRead(A3));
        doc["Data"]["RELAY"] = JsonInteger(digitalRead(7));
        serializeJsonLine(doc);
    }
    else
    {
        Serial.write("error url not found");
        Serial.write(red.c_str());
        serializeJsonLine(doc);
    }
}
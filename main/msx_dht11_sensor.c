typedef struct
{
  uint8_t humidity;
  uint8_t : 1;
  uint8_t temperature;
  uint8_t : 1;
  uint8_t : 1;
} __attribute__((aligned)) dht11_t;
// __attribute__((aligned (5)))

/*
esp_err_t msx_dht11_read(int pin, dht11_t *pack)
{
  uint8_t data[5];
  size_t cnt = 7;
  size_t idx = 0;

  memset(data, 0, 5);

	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

  for (size_t i = 0; digitalRead(pin) == LOW; i++);
  for (size_t i = 0; digitalRead(pin) == HIGH; i++);

  for (size_t i = 0; i < 40; i++)
  {
    for (size_t i = 0; digitalRead(pin) == LOW; i++);
    unsigned long mic = micros();
    for (size_t i = 0; digitalRead(pin) == HIGH; i++);

    if ((micros() - mic) > 40)
    {
      data[idx] |= (1 << cnt);
    }
    if (cnt == 0)
    {
      cnt = 7;
      idx++;
    } else
    {
      cnt--;
    }

  }

	uint8_t humidity    = data[0];
	uint8_t temperature = data[2];

	uint8_t sum = data[0] + data[2];

  memset(pack, 0, 5);
  memcpy(pack, data, 5);

}
*/

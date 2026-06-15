typedef void (*Net_OnReceive)(uint8_t *data, uint8_t length);
void Net_Start(uint8_t deviceID, Net_OnReceive callback, bool isRouter);
bool Net_Update();
void Net_Send(uint8_t ep, uint8_t* data, uint8_t length);
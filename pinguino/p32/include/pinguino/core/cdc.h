#ifdef __cplusplus
extern "C" {
#endif
void USBCheckCDCRequest();
void CDCInitEP();
void USBDeviceInit();		
void USBDeviceAttach();
void putUSBUSART(char *, char);
char getsUSBUSART(char *, char);
void CDCTxService(void);
#ifdef __cplusplus
}
#endif

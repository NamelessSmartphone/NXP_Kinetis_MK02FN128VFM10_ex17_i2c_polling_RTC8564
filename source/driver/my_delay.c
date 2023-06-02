volatile unsigned long SysTickCnt;

void SysTick_Handler (void) {
  SysTickCnt++;
}

void SysTick_Delay (unsigned long tick) {
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}

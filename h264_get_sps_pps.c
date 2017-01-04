static bool_t glk_video_sps_and_pps(char * addr, uint32_t size){
    char sps[512]={0};
    char pps[512]={0};
    char *psps_start = NULL, *ppps_start = NULL, *pidr_start = NULL;
    bool_t sps_start = false, pps_start = false, idr_start = false;
    uint32_t lsps_start = 0, lpps_start = 0, lidr_start = 0;
  
    int i = 0;
  
    char * buf = addr;
    for (i = 0; i < size; i++) {
        if (buf + 4 >= addr + size){
            break;
        }
  
        if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0x01) {
            log_err("type=[%02x]\n", buf[4]);
  
            if (sps_start) {
                sps_start = false;
                lsps_start = buf - psps_start;
                memcpy(sps, psps_start, lsps_start);
                log_err("sps end\n");
            }
  
            if (pps_start) {
                pps_start = false;
                lpps_start = buf - ppps_start;
                memcpy(pps, ppps_start, lpps_start);
                log_err("pps end\n");
            }
  
            if (idr_start) {
                idr_start = false;
                lidr_start = buf - pidr_start;
            }
  
            if ((buf[4] & 0x1F) == 0x07) {
                log_err("find sps\n");
                sps_start = true;
                psps_start = buf;
            } else if ((buf[4] & 0x1F) == 0x08) {
                log_err("find pps\n");
                pps_start = true;
                ppps_start = buf;
            } else if ((buf[4] & 0x1F) == 0x05) {
                log_err("find idr\n");
                idr_start = true;
                pidr_start = buf;
            }
        }
        buf++;
    }
  
    return true;}


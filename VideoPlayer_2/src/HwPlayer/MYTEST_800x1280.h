#include "mi_panel_datatype.h"

#define FLAG_DELAY            0xFE
#define FLAG_END_OF_TABLE     0xFF   // END OF REGISTERS MARKER

#define HPW (4)
#define HBP (80)
#define HFP (80)
#define VPW (4) 
#define VBP (20)
#define VFP (20)

#define FPS (60)
#define HDA (800)
#define VDA (1280)


MI_PANEL_ParamConfig_t stPanelParam =
{
    "MYTESTAPP_800x1280_60", // const char *m_pPanelName;                ///<  PanelName
    0, //MS_U8 m_bPanelDither :1;                 ///<  PANEL_DITHER, keep the setting
    E_MI_PNL_LINK_MIPI_DSI, //MHAL_DISP_ApiPnlLinkType_e m_ePanelLinkType   :4;  ///<  PANEL_LINK

    ///////////////////////////////////////////////
    // Board related setting
    ///////////////////////////////////////////////
    1,  //MS_U8 m_bPanelDualPort  :1;              ///<  VOP_21[8], MOD_4A[1],    PANEL_DUAL_PORT, refer to m_bPanelDoubleClk
    0,  //MS_U8 m_bPanelSwapPort  :1;              ///<  MOD_4A[0],               PANEL_SWAP_PORT, refer to "LVDS output app note" A/B channel swap
    0,  //MS_U8 m_bPanelSwapOdd_ML    :1;          ///<  PANEL_SWAP_ODD_ML
    0,  //MS_U8 m_bPanelSwapEven_ML   :1;          ///<  PANEL_SWAP_EVEN_ML
    0,  //MS_U8 m_bPanelSwapOdd_RB    :1;          ///<  PANEL_SWAP_ODD_RB
    0,  //MS_U8 m_bPanelSwapEven_RB   :1;          ///<  PANEL_SWAP_EVEN_RB

    0,  //MS_U8 m_bPanelSwapLVDS_POL  :1;          ///<  MOD_40[5], PANEL_SWAP_LVDS_POL, for differential P/N swap
    0,  //MS_U8 m_bPanelSwapLVDS_CH   :1;          ///<  MOD_40[6], PANEL_SWAP_LVDS_CH, for pair swap
    0,  //MS_U8 m_bPanelPDP10BIT      :1;          ///<  MOD_40[3], PANEL_PDP_10BIT ,for pair swap
    0,  //MS_U8 m_bPanelLVDS_TI_MODE  :1;          ///<  MOD_40[2], PANEL_LVDS_TI_MODE, refer to "LVDS output app note"

    ///////////////////////////////////////////////
    // For TTL Only
    ///////////////////////////////////////////////
    0,  //MS_U8 m_ucPanelDCLKDelay;                ///<  PANEL_DCLK_DELAY
    0,  //MS_U8 m_bPanelInvDCLK   :1;              ///<  MOD_4A[4],                   PANEL_INV_DCLK
    0,  //MS_U8 m_bPanelInvDE     :1;              ///<  MOD_4A[2],                   PANEL_INV_DE
    0,  //MS_U8 m_bPanelInvHSync  :1;              ///<  MOD_4A[12],                  PANEL_INV_HSYNC
    0,  //MS_U8 m_bPanelInvVSync  :1;              ///<  MOD_4A[3],                   PANEL_INV_VSYNC

    ///////////////////////////////////////////////
    // Output driving current setting
    ///////////////////////////////////////////////
    // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
    1,  //MS_U8 m_ucPanelDCKLCurrent;              ///<  define PANEL_DCLK_CURRENT
    1,  //MS_U8 m_ucPanelDECurrent;                ///<  define PANEL_DE_CURRENT
    1,  //MS_U8 m_ucPanelODDDataCurrent;           ///<  define PANEL_ODD_DATA_CURRENT
    1,  //MS_U8 m_ucPanelEvenDataCurrent;          ///<  define PANEL_EVEN_DATA_CURRENT

    ///////////////////////////////////////////////
    // panel on/off timing
    ///////////////////////////////////////////////
    30,  //MS_U16 m_wPanelOnTiming1;                ///<  time between panel & data while turn on power
    400,  //MS_U16 m_wPanelOnTiming2;                ///<  time between data & back light while turn on power
    80,  //MS_U16 m_wPanelOffTiming1;               ///<  time between back light & data while turn off power
    30,  //MS_U16 m_wPanelOffTiming2;               ///<  time between data & panel while turn off power

    ///////////////////////////////////////////////
    // panel timing spec.
    ///////////////////////////////////////////////
    // sync related
    HPW,  //MS_U8 m_ucPanelHSyncWidth;               ///<  VOP_01[7:0], PANEL_HSYNC_WIDTH
    HBP,  //MS_U8 m_ucPanelHSyncBackPorch;           ///<  PANEL_HSYNC_BACK_PORCH, no register setting, provide value for query only

                                             ///<  not support Manuel VSync Start/End now
                                             ///<  VOP_02[10:0] VSync start = Vtt - VBackPorch - VSyncWidth
                                             ///<  VOP_03[10:0] VSync end = Vtt - VBackPorch
    VPW,  //MS_U8 m_ucPanelVSyncWidth;               ///<  define PANEL_VSYNC_WIDTH
    VBP,  //MS_U8 m_ucPanelVBackPorch;               ///<  define PANEL_VSYNC_BACK_PORCH

    // DE related
    (HPW+HBP),  //MS_U16 m_wPanelHStart;                   ///<  VOP_04[11:0], PANEL_HSTART, DE H Start (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
    (VPW+VBP),  //MS_U16 m_wPanelVStart;                   ///<  VOP_06[11:0], PANEL_VSTART, DE V Start
    HDA,  //MS_U16 m_wPanelWidth;                    ///< PANEL_WIDTH, DE width (VOP_05[11:0] = HEnd = HStart + Width - 1)
    VDA,  //MS_U16 m_wPanelHeight;                   ///< PANEL_HEIGHT, DE height (VOP_07[11:0], = Vend = VStart + Height - 1)

    // DClk related
    0,  //MS_U16 m_wPanelMaxHTotal;                ///<  PANEL_MAX_HTOTAL. Reserved for future using.
    (HDA+HPW+HBP+HFP),//960,  //MS_U16 m_wPanelHTotal;                   ///<  VOP_0C[11:0], PANEL_HTOTAL
    0,  //MS_U16 m_wPanelMinHTotal;                ///<  PANEL_MIN_HTOTAL. Reserved for future using.

    0,  //MS_U16 m_wPanelMaxVTotal;                ///<  PANEL_MAX_VTOTAL. Reserved for future using.
    (VDA+VPW+VBP+VFP),//1300,  //MS_U16 m_wPanelVTotal;                   ///<  VOP_0D[11:0], PANEL_VTOTAL
    0,  //MS_U16 m_wPanelMinVTotal;                ///<  PANEL_MIN_VTOTAL. Reserved for future using.

    ((unsigned long)(VDA+VPW+VBP+VFP)*(HDA+HPW+HBP+HFP)*FPS/1048576),  //MS_U8 m_dwPanelMaxDCLK;                  ///<  PANEL_MAX_DCLK. Reserved for future using.
    ((unsigned long)(VDA+VPW+VBP+VFP)*(HDA+HPW+HBP+HFP)*FPS/1048576),  //MS_U8 m_dwPanelDCLK;                     ///<  LPLL_0F[23:0], PANEL_DCLK          ,{0x3100_10[7:0], 0x3100_0F[15:0]}
    //((unsigned long)(1300)*(960)*FPS/1048576),  //MS_U8 m_dwPanelMaxDCLK;                  ///<  PANEL_MAX_DCLK. Reserved for future using.
    //((unsigned long)(1300)*(960)*FPS/1048576),  //MS_U8 m_dwPanelDCLK;                     ///<  LPLL_0F[23:0], PANEL_DCLK          ,{0x3100_10[7:0], 0x3100_0F[15:0]}
    0,  //MS_U8 m_dwPanelMinDCLK;                  ///<  PANEL_MIN_DCLK. Reserved for future using.
                                             ///<  spread spectrum
    0,  //MS_U16 m_wSpreadSpectrumStep;            ///<  move to board define, no use now.
    0,  //MS_U16 m_wSpreadSpectrumSpan;            ///<  move to board define, no use now.

    160,  //MS_U8 m_ucDimmingCtl;                    ///<  Initial Dimming Value
    255,  //MS_U8 m_ucMaxPWMVal;                     ///<  Max Dimming Value
    80,  //MS_U8 m_ucMinPWMVal;                     ///<  Min Dimming Value

    0,  //MS_U8 m_bPanelDeinterMode   :1;          ///<  define PANEL_DEINTER_MODE,  no use now
    E_MI_PNL_ASPECT_RATIO_WIDE,  //MHAL_DISP_PnlAspectRatio_e m_ucPanelAspectRatio; ///<  Panel Aspect Ratio, provide information to upper layer application for aspect ratio setting.
  /*
    *
    * Board related params
    *
    *  If a board ( like BD_MST064C_D01A_S ) swap LVDS TX polarity
    *    : This polarity swap value =
    *      (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L from board define,
    *  Otherwise
    *    : The value shall set to 0.
    */
    0,  //MS_U16 m_u16LVDSTxSwapValue;
    E_MI_PNL_TI_8BIT_MODE,  //MHAL_DISP_ApiPnlTiBitMode_e m_ucTiBitMode;                         ///< MOD_4B[1:0], refer to "LVDS output app note"
    E_MI_PNL_OUTPUT_8BIT_MODE,  //MHAL_DISP_ApiPnlOutPutFormatBitMode_e m_ucOutputFormatBitMode;

    0,  //MS_U8 m_bPanelSwapOdd_RG    :1;          ///<  define PANEL_SWAP_ODD_RG
    0,  //MS_U8 m_bPanelSwapEven_RG   :1;          ///<  define PANEL_SWAP_EVEN_RG
    0,  //MS_U8 m_bPanelSwapOdd_GB    :1;          ///<  define PANEL_SWAP_ODD_GB
    0,  //MS_U8 m_bPanelSwapEven_GB   :1;          ///<  define PANEL_SWAP_EVEN_GB

    /**
    *  Others
    */
    1,  //MS_U8 m_bPanelDoubleClk     :1;             ///<  LPLL_03[7], define Double Clock ,LVDS dual mode
    0x001c848e,  //MS_U32 m_dwPanelMaxSET;                     ///<  define PANEL_MAX_SET
    0x0018eb59,  //MS_U32 m_dwPanelMinSET;                     ///<  define PANEL_MIN_SET
    E_MI_PNL_CHG_VTOTAL,  //MHAL_DISP_ApiPnlOutTimingMode_e m_ucOutTimingMode;   ///<Define which panel output timing change mode is used to change VFreq for same panel
    0,  //MS_U8 m_bPanelNoiseDith     :1;             ///<  PAFRC mixed with noise dither disable
    (MI_PANEL_ChannelSwapType_e)2,
    (MI_PANEL_ChannelSwapType_e)0,
    (MI_PANEL_ChannelSwapType_e)1,
    (MI_PANEL_ChannelSwapType_e)3,
    (MI_PANEL_ChannelSwapType_e)4,
    /*
    (MI_PANEL_ChannelSwapType_e)2,
    (MI_PANEL_ChannelSwapType_e)4,
    (MI_PANEL_ChannelSwapType_e)3,
    (MI_PANEL_ChannelSwapType_e)1,
    (MI_PANEL_ChannelSwapType_e)0,
    */
};

MI_U8 MYTEST_800x1280[]=
{
0xFF,3,0x98,0x81,0x03,
//GIP_1
0x01,1,0x00,           
0x02,1,0x00,           
0x03,1,0x73,           
0x04,1,0x00,           
0x05,1,0x00,           
0x06,1,0x08,           
0x07,1,0x00,           
0x08,1,0x00,           
0x09,1,0x00,           
0x0A,1,0x01,           
0x0B,1,0x01,           
0x0C,1,0x00,        // STV  DUTY     
0x0D,1,0x01,       //  STV  DUTY         
0x0E,1,0x01,           
0x0F,1,0x00,           
0x10,1,0x00,           
0x11,1,0x00,           
0x12,1,0x00,           
0x13,1,0x1F,        // CLK DUTY        
0x14,1,0x1F,       //  CLK DUTY
0x15,1,0x00,           
0x16,1,0x00,           
0x17,1,0x00,           
0x18,1,0x00,           
0x19,1,0x00,           
0x1A,1,0x00,           
0x1B,1,0x00,           
0x1C,1,0x00,           
0x1D,1,0x00,           
0x1E,1,0x40,           
0x1F,1,0xC0,           
0x20,1,0x06,           
0x21,1,0x01,           
0x22,1,0x06,           
0x23,1,0x01,           
0x24,1,0x88,           
0x25,1,0x88,           
0x26,1,0x00,           
0x27,1,0x00,           
0x28,1,0x3B,           
0x29,1,0x03,           
0x2A,1,0x00,           
0x2B,1,0x00,           
0x2C,1,0x00,           
0x2D,1,0x00,           
0x2E,1,0x00,           
0x2F,1,0x00,           
0x30,1,0x00,           
0x31,1,0x00,           
0x32,1,0x00,           
0x33,1,0x00,           
0x34,1,0x00,  // GPWR1/2 non overlap time 2.62us           
0x35,1,0x00,           
0x36,1,0x00,           
0x37,1,0x00,           
0x38,1,0x00,           
0x39,1,0x00,           
0x3A,1,0x00,           
0x3B,1,0x00,           
0x3C,1,0x00,           
0x3D,1,0x00,           
0x3E,1,0x00,           
0x3F,1,0x00,           
0x40,1,0x00,           
0x41,1,0x00,           
0x42,1,0x00,           
0x43,1,0x00,           
0x44,1,0x00,

//GIP_2           
0x50,1,0x01,           
0x51,1,0x23,           
0x52,1,0x45,           
0x53,1,0x67,           
0x54,1,0x89,           
0x55,1,0xAB,           
0x56,1,0x01,           
0x57,1,0x23,           
0x58,1,0x45,           
0x59,1,0x67,           
0x5A,1,0x89,           
0x5B,1,0xAB,           
0x5C,1,0xCD,           
0x5D,1,0xEF,     

//GIP_3  
0x5E,1,0x00,           
0x5F,1,0x01,           
0x60,1,0x01,           
0x61,1,0x06,           
0x62,1,0x06,           
0x63,1,0x07,           
0x64,1,0x07,           
0x65,1,0x00,           
0x66,1,0x00,           
0x67,1,0x02,           
0x68,1,0x02,           
0x69,1,0x05,           
0x6A,1,0x05,           
0x6B,1,0x02,           
0x6C,1,0x0D,           
0x6D,1,0x0D,           
0x6E,1,0x0C,           
0x6F,1,0x0C,           
0x70,1,0x0F,           
0x71,1,0x0F,           
0x72,1,0x0E,           
0x73,1,0x0E,           
0x74,1,0x02,           
0x75,1,0x01,           
0x76,1,0x01,           
0x77,1,0x06,           
0x78,1,0x06,           
0x79,1,0x07,           
0x7A,1,0x07,           
0x7B,1,0x00,           
0x7C,1,0x00,           
0x7D,1,0x02,           
0x7E,1,0x02,           
0x7F,1,0x05,           
0x80,1,0x05,           
0x81,1,0x02,           
0x82,1,0x0D,           
0x83,1,0x0D,           
0x84,1,0x0C,           
0x85,1,0x0C,           
0x86,1,0x0F,           
0x87,1,0x0F,           
0x88,1,0x0E,           
0x89,1,0x0E,           
0x8A,1,0x02,

//Page 4 command;           
0xFF,3,0x98,0x81,0x04,           
 
//3B,01,C0     // ILI4003D sel
0x6C,1,0x15,        //Set VCORE voltage =1.5V           
0x6E,1,0x2A,        //di_pwr_reg=0 for power mode 2A //VGH clamp 18V           
0x6F,1,0x33,    //45 //pumping ratio VGH=5x VGL=-3x                     
0x8D,1,0x1B,        //VGL clamp -10V           
0x87,1,0xBA,        //ESD  
0x3A,1,0x24,        //POWER SAVING                     
0x26,1,0x76,           
0xB2,1,0xD1,           
   

 // Page 1 command             
0xFF,3,0x98,0x81,0x01,           
0x22,1,0x0A,        //BGR, SS           
0x31,1,0x00,        //Zigzag type3 inversion           
0x43,1,0x66,                 
0x53,1,0x42,                   
0x50,1,0x87,                    
0x51,1,0x82,                     
0x60,1,0x15,  
0x61,1,0x01, 
0x62,1,0x0C, 
0x63,1,0x00,          

// Gamma P      
0xA0,1,0x00,    
0xA1,1,0x13,        //VP251         
0xA2,1,0x23,        //VP247 
0xA3,1,0x14,        //VP243         
0xA4,1,0x16,        //VP239         
0xA5,1,0x29,        //VP231         
0xA6,1,0x1E,        //VP219         
0xA7,1,0x1D,        //VP203         
0xA8,1,0x86,        //VP175         
0xA9,1,0x1E,        //VP144         
0xAA,1,0x29,        //VP111         
0xAB,1,0x74,        //VP80          
0xAC,1,0x19,        //VP52          
0xAD,1,0x17,        //VP36          
0xAE,1,0x4B,        //VP24          
0xAF,1,0x20,        //VP16          
0xB0,1,0x26,        //VP12          
0xB1,1,0x4C,        //VP8           
0xB2,1,0x5D,        //VP4           
0xB3,1,0x3F,        //VP0          

// Gamma N      
0xC0,1,0x00,        //VN255 GAMMA N           
0xC1,1,0x13,        //VN251         
0xC2,1,0x23,        //VN247         
0xC3,1,0x14,        //VN243         
0xC4,1,0x16,        //VN239         
0xC5,1,0x29,        //VN231         
0xC6,1,0x1E,        //VN219         
0xC7,1,0x1D,        //VN203         
0xC8,1,0x86,        //VN175         
0xC9,1,0x1E,        //VN144         
0xCA,1,0x29,        //VN111         
0xCB,1,0x74,        //VN80          
0xCC,1,0x19,        //VN52          
0xCD,1,0x17,        //VN36          
0xCE,1,0x4B,        //VN24          
0xCF,1,0x20,        //VN16          
0xD0,1,0x26,        //VN12          
0xD1,1,0x4C,        //VN8           
0xD2,1,0x5D,        //VN4           
0xD3,1,0x3F,        //VN0

// Page 0 command           
0xFF,3,0x98,0x81,0x00,
0x35,1,0x00,  // TE On
0x11,0,0x00,  // Sleep Out          
FLAG_DELAY,FLAG_DELAY,120,
0x29,0,0x00,  //  Display On 
FLAG_DELAY,FLAG_DELAY,20,

//FLAG_END_OF_TABLE, FLAG_END_OF_TABLE,	
};

MI_PANEL_MipiDsiConfig_t stMipiDsiConfig =
{
    //HsTrail HsPrpr HsZero ClkHsPrpr ClkHsExit ClkTrail ClkZero ClkHsPost DaHsExit ContDet
    //5,      3,     5,     10,       14,       3,       12,     10,       5,       0,
      4,      5,     5,     4,        6,        4,       12,     10,       6,       0,
    //Lpx   TaGet  TaSure  TaGo
      4,   15,    5,     12,

    //Hac,  Hpw,  Hbp, Hfp, Vac,  Vpw,  Vbp, Vfp,  Bllp, Fps
      HDA,  HPW,  HBP, HFP, VDA, VPW,  VBP, VFP,  0,    FPS,

    E_MI_PNL_MIPI_DSI_LANE_4,      // MIPnlMipiDsiLaneMode_e enLaneNum;
    E_MI_PNL_MIPI_DSI_RGB888,      // MIPnlMipiDsiFormat_e enFormat;
    E_MI_PNL_MIPI_DSI_SYNC_PULSE,  // MIPnlMipiDsiCtrlMode_e enCtrl;

    MYTEST_800x1280,
    sizeof(MYTEST_800x1280),
    1, 0x01AF, 0x01B9, 0x80D2, 9,
};

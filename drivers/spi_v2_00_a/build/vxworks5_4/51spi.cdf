
/*******************************************************************************
* SPI component and parameters
*******************************************************************************/

Folder FOLDER_xtag_csp_SPI {
  NAME       SPI
  SYNOPSIS   SPI Support
  _CHILDREN  FOLDER_xtag_csp_CSP
}

Component INCLUDE_XSPI {
  NAME       SPI interface
  SYNOPSIS   SPI interface
  REQUIRES   
  _CHILDREN  FOLDER_xtag_csp_SPI
}

#pragma once
// gestion afficheur LCD
// LCD I2C ou standard

#ifdef LiquidCrystal_h
#define LCD_CLASS LiquidCrystal
#endif
#ifdef LiquidCrystal_I2C_h
#define LCD_CLASS LiquidCrystal_I2C
#endif
#ifndef LCD_CLASS
#error "Vous devez selectionner  LiquidCrystal.h ou LiquidCrystal_I2C.h en fonction de votre écran dans config.h"
#endif



void pgm_read_8byte(const byte *data, void *buf) {
  uint32_t *buf_ = (uint32_t *)buf;

  buf_[0] = pgm_read_dword(data + 0);
  buf_[1] = pgm_read_dword(data + 4);
}

class LiquidCrystalCyr : public LCD_CLASS {
public:
#ifdef LiquidCrystal_h
  LiquidCrystalCyr(uint8_t rs, uint8_t enable,
                   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                   uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7){};
  LiquidCrystalCyr(uint8_t rs, uint8_t rw, uint8_t enable,
                   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                   uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7){};
  LiquidCrystalCyr(uint8_t rs, uint8_t rw, uint8_t enable,
                   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
    : LiquidCrystal(rs, rw, enable, d0, d1, d2, d3){};
  LiquidCrystalCyr(uint8_t rs, uint8_t enable,
                   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
    : LiquidCrystal(rs, enable, d0, d1, d2, d3){};
#endif
#ifdef LiquidCrystal_I2C_h
  LiquidCrystalCyr(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows), nCols(lcd_cols), nRows(lcd_rows) {
  }
#endif
  uint8_t nCols = 0;
  uint8_t nRows = 0;

  void begin(uint8_t cols, uint8_t rows) {


    nCols = cols;
    nRows = rows;
#ifdef LiquidCrystal_h
    LCD_CLASS::begin(cols, rows, LCD_5x8DOTS);
#endif
#ifdef LiquidCrystal_I2C_h
    init();
    backlight();
#endif
  }

  void clear() {
    _col = 0;
    _row = 0;
    return LCD_CLASS::clear();
  }

  void setCursor(uint8_t col, uint8_t row) {
    _col = col;
    _row = row;
    return LCD_CLASS::setCursor(col, row);
  }

  byte *customChars_[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

  void createChar(uint8_t location, uint8_t charmap[]) {
    customChars_[location] = charmap;  // comportement standard modifié - le tableau de caractères transmis doit exister en permanence
  }

 

  void printAt(uint8_t col, uint8_t row, char ch) {
    setCursor(col, row);
    write(byte(ch));
  }

  void printAt(uint8_t col, uint8_t row, const char *s) {
    setCursor(col, row);
    print(s);
  }

  void printAt_P(uint8_t col, uint8_t row, PGM_P s) {
    setCursor(col, row);

    char buf[strlen_P(s) + 1];
    strcpy_P(buf, s);
    print(buf);
  }


  void printfAt(uint8_t col, uint8_t row, const char *format, ...) {
    setCursor(col, row);

    char buf[21];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, 21, format, args);
    va_end(args);
    print(buf);
  }

  void printfAt_P(uint8_t col, uint8_t row, PGM_P format, ...) {
    setCursor(col, row);

    char buf[21];
    va_list args;
    va_start(args, format);
    vsnprintf_P(buf, 21, format, args);
    va_end(args);
    print(buf);
  }

private:
  byte _row = 0;
  byte _col = 0;

  bool _CGRAM_write = 0;

  byte _query[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };

  // le tableau stocke le numéro de l'emplacement  LCD où le générateur de caractères est chargé (0..7)
  //byte _gen[GEN_COUNT];

  // abandon d un espace d'affichage libre ou libéré
  byte get_char_cell(byte lcd_c) {
    byte i = 0;

    if (!lcd_c)
      i = 7;

    while ((i < 7) && (_query[i] != lcd_c))
      ++i;

    byte r = _query[i];

    for (; i > 0; --i)
      _query[i] = _query[i - 1];
    _query[0] = r;

    return r;
  }
};

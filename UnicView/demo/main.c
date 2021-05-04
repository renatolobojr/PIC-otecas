#include <main.h>
//#include <stdint.h>
//#include "unicviewad_c.h"
#include <unicviewad_c.c>

volatile uint8_t lcm_rx_ring_buffer_index = 0;

volatile uint8_t lcm_rx_ring_buffer_received_index = 0;
volatile uint8_t lcm_rx_ring_buffer[128];

volatile uint32_t millis;

void digital_write(uint16_t pin, uint8_t value)
{
   if (value == TRUE)
   {
      output_high(pin);
   }
   else
   {
      output_low(pin);
   }
}

#INT_TIMER2
void  TIMER2_isr(void)
{
   millis++;
}

#INT_RDA
void serial_data_receive()
{
   lcm_rx_ring_buffer[lcm_rx_ring_buffer_index] = getc(lcm_serial);

   ++lcm_rx_ring_buffer_index;

   IF(128 == lcm_rx_ring_buffer_index)
   {
      lcm_rx_ring_buffer_index = 0;
   }
}

uint8_t lcm_write_bytes(uint8_t* data, uint8_t length) // Fun��o de escrita na serial utilizada pela biblioteca UnicViewAD.lcm
{
   FOR(uint8_t i = 0; i < length; i++)
   {
      fputc(data[i], lcm_serial);
   }

   RETURN length;
}

uint16_t lcm_read_byte()             // Fun��o de leitura da serial utilizada pela biblioteca UnicViewAD.lcm
{

   IF(lcm_rx_ring_buffer_index != lcm_rx_ring_buffer_received_index)
   {
      uint8_t data = lcm_rx_ring_buffer[lcm_rx_ring_buffer_received_index];
      ++lcm_rx_ring_buffer_received_index;
      if (128 == lcm_rx_ring_buffer_received_index)
      {
         lcm_rx_ring_buffer_received_index = 0;
      }
      RETURN data;
   }
   ELSE
   {
      RETURN LCM_EMPTY_RESPONSE;
   }

}


/*******************************************************************************************************************************************
 * �ndice das telas (Screens)
 */
 /*
  * Os valores atribu�dos �s antes s�o os "PicIds".
  * Os "PicIds" s�o definidos no projeto DO UnicView AD.
  */
uint8_t pic_id_intro = 0;                   // PicId da tela de introdu��o              (PicId 00 - Intro    )
uint8_t pic_id_audio = 1;                   // PicId da tela de �udio                   (PicId 01 - Audio    )
uint8_t pic_id_rtc = 2;                     // PicId da tela do RTC                     (PicId 02 - RTC      )
uint8_t pic_id_curve_a0 = 3;                 // PicId da tela de curva A0                (PicId 03 - CurveA0  )
uint8_t pic_id_curve_simulation = 4;         // PicId da tela de simula��o da curva      (PicId 04 - CurveSim )
uint8_t pic_id_pressure = 5;                // PicId da tela de press�o                 (PicId 05 - Pressure )
uint8_t pic_id_leds = 6;                    // PicId da tela de LED                     (PicId 06 - Led      )
uint8_t pic_id_text = 7;                    // PicId da tela de texto                   (PicId 07 - Text     )
uint8_t pic_id_settings = 8;                // PicId da tela de ajustes                 (PicId 08 - Settings )
uint8_t pic_id_volume = 9;                  // PicId da tela de volume                  (PicId 09 - Volume   )
uint8_t pic_id_main = 10;                   // PicId da tela principal                  (PicId 10 - Main     )
/*******************************************************************************************************************************************
 * Vari�veis da tela de introdu��o (PicId 0 - intro)
 */
uint16_t init_demo = 1000;                  // "RETURN Value" do �nico bot�o da tela de introdu��o (A tela inteira � o bot�o)
/*******************************************************************************************************************************************
 * Vari�veis da tela principal (PicId 10 - Main)
 */
#define NAVIGATE_TO_AUDIO 1              // "RETURN Value" do bot�o de navega��o para o �udio (PicId 1 - Audio)
#define NAVIGATE_TO_RTC 2                // "RETURN Value" do bot�o de navega��o para o RTC (PicId 2 - RTC)
#define NAVIGATE_TO_CURVE_A0 3            // "RETURN Value" do bot�o de navega��o para o curva A0 (PicId 3 - CurveA0)
#define NAVIGATE_TO_CURVE_SIMULATION 4    // "RETURN Value" do bot�o de navega��o para o simula��o de curvas (PicId 4 - CurveSim)
#define NAVIGATE_TO_PRESSURE 5           // "RETURN Value" do bot�o de navega��o para o press�o (PicId 5 - Pressure)
#define NAVIGATE_TO_LED 6                // "RETURN Value" do bot�o de navega��o para o LED (PicId 6 - Led)
#define NAVIGATE_TO_TEXT 7               // "RETURN Value" do bot�o de navega��o para o texto (PicId 7 - Text)
#define NAVIGATE_TO_SETTINGS 8           // "RETURN Value" do bot�o de navega��o para o ajustes  (PicId 8 - Settings)
 /*******************************************************************************************************************************************
  * Vari�veis da tela de �udio (PicId 1 - Audio)
  */
#define NAVIGATE_TO_VOLUME 111           // "RETURN Value" do bot�o de navega��o para o volume (PicId 9 - Volume)
#define AUDIO_CONTROLS_MUTE 110          // "RETURN Value" do bot�o de controle da tela de �udio (PicId 1 - Audio) - Ativa / desativa Mudo
#define AUDIO_CONTROLS_REPEAT 120        // "RETURN Value" do bot�o de controle da tela de �udio (PicId 1 - Audio) - Ativa / desativa repetir
#define AUDIO_CONTROLS_PREVIOUS 121      // "RETURN Value" do bot�o de controle da tela de �udio (PicId 1 - Audio) - �udio anterior
#define AUDIO_CONTROLS_PLAY_PAUSE 122     // "RETURN Value" do bot�o de controle da tela de �udio (PicId 1 - Audio) - Play / pause
#define AUDIO_CONTROLS_NEXT 123          // "RETURN Value" do bot�o de controle da tela de �udio (PicId 1 - Audio) - Pr�ximo �udio
#define AUDIO_CONTROLS_STOP 124          // "RETURN Value" do bot�o de controle da tela de �udio (PicId 1 - Audio) - Para a reprodu��o
#define QUANTITY_OF_TRACKS 27            // Quantidade de �udios
uint8_t current_track = 1;                       // Armazena o valor DO �udio atual
BOOLEAN is_muted = FALSE;                       // Armazena a situa��o DO controle mudo | ativado/desativado
BOOLEAN is_repeating = FALSE;                   // Armazena a situa��o DO controle de repeti��o | ativado/desativado
BOOLEAN previous_disabled = TRUE;               // Armazena a situa��o DO controle de retornar um �udio | habilitade/desabilitado
BOOLEAN is_playing = FALSE;                     // Armazena a situa��o DO controle de reprodu��o de �udio | play / pause
BOOLEAN next_disabled = FALSE;                  // Armazena a situa��o DO controle de avan�ar um �udio | habilitade/desabilitado
BOOLEAN is_stopped = TRUE;                      // Armazena a situa��o DO controle de parar  a reprodu��o de �udio
const uint8_t muted_volume = 0;                  // valor enviado para o volume quando o �udio est�o com o mudo ativo
/*******************************************************************************************************************************************
 * Vari�veis da tela de RTC (PicId 2 - RTC)
 */
uint8_t software_control_code_day = 20;      // "Software Control Code" do "Numeric Input" que altera o valor do dia
uint8_t software_control_code_month = 21;    // "Software Control Code" do "Numeric Input" que altera o valor do m�s
uint8_t software_control_code_year = 22;     // "Software Control Code" do "Numeric Input" que altera o valor do ano
uint8_t software_control_code_hour = 23;     // "Software Control Code" do "Numeric Input" que altera o valor da hora
uint8_t software_control_code_minute = 24;   // "Software Control Code" do "Numeric Input" que altera o valor do minuto
uint8_t software_control_code_second = 25;   // "Software Control Code" do "Numeric Input" que altera o valor do segundo
uint8_t rtc_selected_icon_none = 0;          // Valor usado para desativar o efeito de sele��o do RTC
uint8_t rtc_selected_icon_day = 1;           // Valor usado para ativar o efeito de sele��o do campo dia
uint8_t rtc_selected_icon_month = 2;         // Valor usado para ativar o efeito de sele��o do campo m�s
uint8_t rtc_selected_icon_year = 3;          // Valor usado para ativar o efeito de sele��o do campo ano
uint8_t rtc_selected_icon_hour = 4;          // Valor usado para ativar o efeito de sele��o do campo hora
uint8_t rtc_selected_icon_minute = 5;        // Valor usado para ativar o efeito de sele��o do campo minuto
uint8_t rtc_selected_icon_second = 6;        // Valor usado para ativar o efeito de sele��o do campo segundo
#define RTC_BUTTON_DAY 240               // "RETURN Value" do bot�o para alterar o valor do dia
#define RTC_BUTTON_MONTH 241             // "RETURN Value" do bot�o para alterar o valor do m�s
#define RTC_BUTTON_YEAR 242              // "RETURN Value" do bot�o para alterar o valor do ano
#define RTC_BUTTON_HOUR 243              // "RETURN Value" do bot�o para alterar o valor do hora
#define RTC_BUTTON_MINUTE 244            // "RETURN Value" do bot�o para alterar o valor do minuto
#define RTC_BUTTON_SECOND 245            // "RETURN Value" do bot�o para alterar o valor do segundo
#define RTC_BUTTON_DATE 246              // "RETURN Value" do bot�o para alterar os valores da data | dia / m�s / ano
#define RTC_BUTTON_TIME 247              // "RETURN Value" do bot�o para alterar os valores da hora | hora / minuto / segundo
uint8_t keyboard_is_open = FALSE;                        // Armazena a situa��o DO teclado do RTC | aberto / fechado
uint32_t last_keyboard_response_time = 0;              // Armazena um valor de tempo usado para dar um espa�o de tempo entre as verifica��es do teclado
uint16_t keyboard_is_open_read_interval_in_ms = 1000;    // Espa�o de tempo (em milissegundos) entre as verifica��es de teclado
uint8_t last_rtc_day;                             // Armazena o �ltimo valor configurado para o dia
uint8_t last_rtc_month;                           // Armazena o �ltimo valor configurado para o m�s
uint8_t last_rtc_year;                            // Armazena o �ltimo valor configurado para o ano
uint16_t last_rtc_hour;                            // Armazena o �ltimo valor configurado para a hora
uint8_t last_rtc_minute;                          // Armazena o �ltimo valor configurado para o minuto
uint8_t last_rtc_second;                          // Armazena o �ltimo valor configurado para o segundo
uint8_t current_rtc_keyboard = 0;                 // Armazena o valor atual de qual campo DO RTC est� sendo alterado
uint8_t rtc_keyboard_sequence_none = 0;      // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - Sem sequ�ncia
uint8_t rtc_keyboard_sequence_day = 1;       // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - Dia
uint8_t rtc_keyboard_sequence_month = 2;     // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - M�s
uint8_t rtc_keyboard_sequence_year = 3;      // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - Ano
uint8_t rtc_keyboard_sequence_hour = 4;      // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - Hora
uint8_t rtc_keyboard_sequence_minute = 5;    // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - minuto
uint8_t rtc_keyboard_sequence_second = 6;    // Valor atual do �ndice sequ�ncial para quando for atribuir mais de um valor no RTC - Segundo

/*******************************************************************************************************************************************
 * Vari�veis da tela de curva A0 (PicId 3 - CurveA0)
 */
#define CURVE_A0_CONTROL_PLAY 300         // "RETURN Value" do bot�o de controle da tela de curva A0 (PicId 3 - CurveA0) - Inicia a leitura anal�gica do pino A0
#define CURVE_A0_CONTROL_PAUSE 301        // "RETURN Value" do bot�o de controle da tela de curva A0 (PicId 3 - CurveA0) - Pausa a leitura anal�gica do pino A0
#define CURVE_A0_CONTROL_STOP 302         // "RETURN Value" do bot�o de controle da tela de curva A0 (PicId 3 - CurveA0) - Para a leitura anal�gica do pino A0 e apaga todos os pontos lidos
BOOLEAN curve_a0_is_plotting = FALSE;             // Armazena se est�o sendo realizado a leitura anal�gica DO pino A0
uint8_t curve_a0_analog_pin = PIN_A0;            // Define qual pino amal�gico ser� lido
uint32_t last_curve_a0_write_time = 0;          // Armazena um valor de tempo usado para dar um espa�o de tempo entre as leituras do pino anal�gico
uint8_t curve_a0_write_interval_in_ms = 100;   // Espa�o de tempo entre as leituras do pino anal�gico
/*******************************************************************************************************************************************
 * Vari�veis da tela de simula��o da curva (PicId 4 - CurveSim)
 */
#define CURVE_SIMULATION_SAMPLE_ECG 410   // "RETURN Value" do bot�o de controle da tela de simula��o da curva (PicId 4 - CurveSim) - envia um exemplo de ECG
#define CURVE_SIMULATION_SAMPLE_SINE 411  // "RETURN Value" do bot�o de controle da tela de simula��o da curva (PicId 4 - CurveSim) - envia um exemplo de senoide

uint8_t curve_number_of_points = 216;        // Quantidade m�xima de pontos do gr�fico da tela de simula��o e curva (PicId 4 - CurveSim)

uint8_t ecg_sample_points[] =
{
   // Vetor com o exemplo de um tra�ado de ECG
   39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 41, 44, 47, 50, 52, 52, 48, 43, 41, 39, 39, 39, 37, 33, 38,
   45, 60, 75, 90, 100, 94, 80, 63, 46, 29, 17, 11, 17, 27, 34, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39
};

uint8_t ecg_sample_points_length = 55;       // Quantidade de pontos no exemplo de ECG

uint8_t sine_sample_points[] =
{
   50, 55, 60, 65, 69, 74, 78, 82, 86, 89, 92, 95, 97, 98, 99, 100, 100, 100, 99,
   97, 95, 93, 90, 87, 84, 80, 76, 71, 67, 62, 57, 52, 47, 42, 37, 32, 28, 24, 19,
   16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 8, 11, 15, 18, 22, 27, 31, 36, 41, 46
};

uint8_t sine_sample_points_length = 63;       // Quantidade de pontos no exemplo de ECG

uint8_t ecg_sample_activated = 0;
uint16_t ecg_curve_current_point = 0;
uint8_t ecg_sample_index = 0;
uint8_t sine_sample_activated = 0;
uint16_t sine_curve_current_point = 0;
uint8_t sine_sample_index = 0;

/*******************************************************************************************************************************************
 * Vari�veis da tela de press�o (PicId 5 - Pressure)
 */
uint8_t pressure_alert_level = 90;          // Valor da press�o que ser� ativado o icone de alerta
uint8_t pressure_setpoint_value = 0;              // Vari�vel que receber� o novo valor de press�o
uint8_t current_pressure_value = 50;              // Vari�vel de controle da press�o atual
BOOLEAN adjust_pressure = FALSE;                // Vari�vel que armazena a situa��o se a press�o precisa ser ajustada
uint32_t last_pressure_write_time = 0;         // Armazena um valor de tempo usado para dar um espa�o de tempo entre as Atualiza��es do valor da press�o
uint8_t pressure_write_interval_in_ms = 100;  // Espa�o de tempo entre os ajustes de press�o
/*******************************************************************************************************************************************
 * Vari�veis da tela de LED (PicId 6 - Led)
 */
#define LED_CONTROLS_TOGGLE_LED1 611          // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - Altera o estado do led 1 liga/desliga
#define LED_CONTROLS_TOGGLE_LED2 612          // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - Altera o estado do led 2 liga/desliga
#define LED_CONTROLS_TOGGLE_LED3 613          // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - Altera o estado do led 3 liga/desliga
#define LED_CONTROLS_TOGGLE_LED4 614          // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - Altera o estado do led 4 liga/desliga
#define LED_CONTROLS_SET_ALL_LEDS_TO_LOW 615     // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - Desliga todos os leds
#define LED_CONTROLS_TOGGLE_ALL_LEDS 616       // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - Altera o estado de todos os leds liga/desliga
#define LED_CONTROLS_SET_ALL_LEDS_TO_HIGH 617    // "RETURN Value" do bot�o de controle da tela de LED (PicId 6 - Led) - liga todos os leds
#define LED_PIN1 PIN_A1                      // Defini��o do pino que ser� ligado o led 1
#define LED_PIN2 PIN_A2                      // Defini��o do pino que ser� ligado o led 2
#define LED_PIN3 PIN_A3                      // Defini��o do pino que ser� ligado o led 3
#define LED_PIN4 PIN_A4                      // Defini��o do pino que ser� ligado o led 4
 /*******************************************************************************************************************************************
  * Vari�veis da tela de texto (PicId 7 - Text)
  */
#define TEXT_LENGTH_IN_CHARS 12           // Tamanho da caixa de texto da tela de texto (PicId 7 - Text) em caracteres.
  /*******************************************************************************************************************************************
   * Vari�veis da tela de ajustes (PicId 8 - Settings)
   */
uint8_t popup_cancel = 240;                // "RETURN Value" do bot�o de controle da tela de ajustes   (PicId 8 - Settings) - Popup - CANCELAR
uint8_t popup_confirm = 241;               // "RETURN Value" do bot�o de controle da tela de ajustes   (PicId 8 - Settings) - Popup - CONFIRMAR
/*******************************************************************************************************************************************
 * Vari�veis da tela de volume (PicId 9 - Volume)
 */
uint8_t volume = 7;                             // Armazena o volume de reprodu��o DO �udio
#define NAVIGATE_FROM_VOLUME 9           // "RETURN Value" do bot�o de navega��o voltando da tela de volume (PicId 9 - Volume)
/*******************************************************************************************************************************************
 * Inicializa��o dos LcmVars
 */

lcm_var_parameters_t exit_intro_button = { 0, TWO_BYTES };                 // "LcmVar" DO bot�o da tela de introdu��o (PicId 0 - Intro)
lcm_var_parameters_t navigation_buttons = { 1, TWO_BYTES };               // "LcmVar" dos bot�es de navega��o da tela principal (PicId 10 - main)
lcm_var_parameters_t pic_id_indicator = { 10, TWO_BYTES };                 // "LcmVar" dos campos que indicam o PicId atual
lcm_var_parameters_t audio_controls = { 100, TWO_BYTES };                 // "LcmVar" dos bot�es de controle da tela de �udio (PicId 1 - Audio)
lcm_var_parameters_t audio_volume = { 900, TWO_BYTES };                   // "LcmVar" DO controle de volume da tela de volume (PicId 9 - Volume)
lcm_var_parameters_t audio_repeat_indicator = { 101, TWO_BYTES };          // "LcmVar" DO �cone que indica se os �udio est�o para repetir
lcm_var_parameters_t audio_previous_indicator = { 102, TWO_BYTES };        // "LcmVar" DO �cone que indica se � poss�vel voltar um �udio
lcm_var_parameters_t audio_play_pause_indicator = { 103, TWO_BYTES };       // "LcmVar" DO �cone que indica se quando pressionar o play/pause, ir� iniciar ou pausar a reprodu��o
lcm_var_parameters_t audio_next_indicator = { 104, TWO_BYTES };            // "LcmVar" DO �cone que indica se � poss�vel avan�ar um �udio
lcm_var_parameters_t audio_stop_indicator = { 105, TWO_BYTES };            // "LcmVar" DO �cone que indica a possibilidade de parar a reprodu��o do �udio
lcm_var_parameters_t audio_current_track_indicator = { 110, TWO_BYTES };    // "LcmVar" que mostra o �udio que est� selecionado
lcm_var_parameters_t audio_last_track_indicator = { 111, TWO_BYTES };       // "LcmVar" que mostra o �ltimo �udio possivel de ser selecionado
lcm_var_parameters_t rtc_buttons = { 201, TWO_BYTES };                    // "LcmVar" dos bot�es de controle que definem o RTC
lcm_var_parameters_t rtc_selected_icons = { 200, TWO_BYTES };              // "LcmVar" dos icones que podem ser selecionados para altera��o
lcm_var_parameters_t keyboard_day = { 210, TWO_BYTES };                   // "LcmVar" DO teclado que altera o valor do Dia
lcm_var_parameters_t keyboard_month = { 211, TWO_BYTES };                 // "LcmVar" DO teclado que altera o valor do M�s
lcm_var_parameters_t keyboard_year = { 212, TWO_BYTES };                  // "LcmVar" DO teclado que altera o valor do Ano
lcm_var_parameters_t keyboard_hour = { 213, TWO_BYTES };                  // "LcmVar" DO teclado que altera o valor da Hora
lcm_var_parameters_t keyboard_minute = { 214, TWO_BYTES };                // "LcmVar" DO teclado que altera o valor do Minuto
lcm_var_parameters_t keyboard_second = { 215, TWO_BYTES };                // "LcmVar" DO teclado que altera o valor do Segundo
lcm_var_parameters_t curve_a0_controls = { 301, TWO_BYTES };               // "LcmVar" dos bot�es de controle da tela curva A0 (PicId 3 - CurveA0)
lcm_var_parameters_t curve_a0_temperature_indicator = { 300, TWO_BYTES };   // "LcmVar" dos icones que mostram a temperatura no term�metro da tela de curva A0 (PicId 3 - CurveA0)
lcm_var_parameters_t curve_simulation_sample_buttons = { 400, TWO_BYTES };          // "LcmVar" dos exemplos de curvas da tela de simula��o da curva (PicId 4 - CurveSim)
lcm_var_parameters_t curve_simulation_manual_input = { 410, TWO_BYTES };            // "LcmVar" DO Knob de controle de temperatura da tela de simula��o da curva (PicId 4 - CurveSim)
lcm_var_parameters_t curve_simulation_temperature_indicator = { 420, TWO_BYTES };   // "LcmVar" dos �cones que mostram a temperatura no term�metro da tela de simula��o da curva (PicId 4 - CurveSim)
lcm_var_parameters_t pressure_setpoint = { 500, TWO_BYTES };              // "LcmVar" da press�o configurada
lcm_var_parameters_t current_pressure_indicator = { 501, TWO_BYTES };      // "LcmVar" DO �cone de ponteiro central da press�o
lcm_var_parameters_t pressure_alert_indicator = { 502, TWO_BYTES };        // "LcmVar" DO �cone de alerta de alta press�o
lcm_var_parameters_t led_controls = { 600, TWO_BYTES };                   // "LcmVar" dos controles de LED da tela de LED (PicId 6 - Led)
lcm_var_parameters_t led1_indicator = { 601, TWO_BYTES };                 // "LcmVar" DO �cone que indica se o LED 1 est� ligado
lcm_var_parameters_t led2_indicator = { 602, TWO_BYTES };                 // "LcmVar" DO �cone que indica se o LED 2 est� ligado
lcm_var_parameters_t led3_indicator = { 603, TWO_BYTES };                 // "LcmVar" DO �cone que indica se o LED 3 est� ligado
lcm_var_parameters_t led4_indicator = { 604, TWO_BYTES };                 // "LcmVar" DO �cone que indica se o LED 4 est� ligado
lcm_string_parameters_t text_input = { 700, TEXT_LENGTH_IN_CHARS };    // "LcmString" da entrada de texto da tela de texto (PicId 7 - Text)
lcm_string_parameters_t inverted_text = { 720, TEXT_LENGTH_IN_CHARS }; // "LcmString" DO campo de texto invertido da tela de texto (PicId 7 - Text)
lcm_var_parameters_t back_light = { 800, TWO_BYTES };                     // "LcmVar" da entrada de configura��o DO backlight do display
lcm_var_parameters_t read_version_button = { 801, TWO_BYTES };             // "LcmVar" DO bot�o de leitura da vers�o do firmware do display
lcm_var_parameters_t version_indicator = { 810, TWO_BYTES };              // "LcmVar" DO campo de texto que mostra a vers�o do firmware do display
lcm_var_parameters_t activate_buzzer_button = { 802, TWO_BYTES };          // "LcmVar" DO bot�o que toca o buzzer
lcm_var_parameters_t buzzer_activation_time = { 803, TWO_BYTES };          // "LcmVar" DO valor de configura��o do tempo de toque do buzzer
lcm_var_parameters_t calibrate_button = { 804, TWO_BYTES };               // "LcmVar" DO bot�o de calibra��o
lcm_var_parameters_t reset_button = { 805, TWO_BYTES };                   // "LcmVar" DO bot�o de resetar o display


VOID lcm_picid_0_intro();
VOID lcm_picid_10_main();
VOID lcm_picid_1_audio();
VOID lcm_picid_9_volume();
VOID lcm_picid_2_rtc();
VOID lcm_picid_3_adc_curve();
VOID lcm_picid_4_curve_simulation();
VOID lcm_picid_5_pressure();
VOID lcm_picid_6_led();
VOID lcm_picid_7_text();
VOID lcm_picid_8_settings();

uint32_t millis_test = 0;

VOID main()
{
   setup_timer_2(T2_DIV_BY_4, 249, 5);      //200 us overflow, 1,0 ms interrupt
   setup_adc(ADC_CLOCK_DIV_16);
   setup_adc_ports(AN0);
   lcm_init();

   digital_write(LED_PIN1, LOW);
   digital_write(LED_PIN2, LOW);
   digital_write(LED_PIN3, LOW);
   digital_write(LED_PIN4, LOW);

   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_RDA);


   WHILE(true)
   {
      lcm_picid_0_intro();
      lcm_picid_10_main();
      lcm_picid_1_audio();
      lcm_picid_9_volume();
      lcm_picid_2_rtc();
      lcm_picid_3_adc_curve();
      lcm_picid_4_curve_simulation();
      lcm_picid_5_pressure();
      lcm_picid_6_led();
      lcm_picid_7_text();
      lcm_picid_8_settings();

   }
}

VOID lcm_picid_0_intro()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de introdu��o (PicId 0 - Intro)
   */
   IF(lcm_var_available(&exit_intro_button)) // Verifica se chegou algo no "LcmVar" "ExitIntroButton"
   {
      if (lcm_var_get_data(&exit_intro_button) == init_demo) // Verifica se o valor recebido pelo "LcmVar" "ExitIntroButton" � o "RETURN Value" do bot�o da tela de introdu��o (PicId 0 - Intro) - que ir� atribuir os valores a todos os campos
      {
         lcm_change_pic_id(pic_id_main);  // Muda a tela para a tela principal (PicId 10 - main)

         lcm_var_write(&audio_repeat_indicator, is_repeating); // Escreve no "LcmVar" "AudioRepeatIndicator" o valor da vari�vel "isRepeating"
         lcm_var_write(&audio_previous_indicator, previous_disabled); // Escreve no "LcmVar" "AudioPreviousIndicator" o valor da vari�vel "previousDisabled"
         lcm_var_write(&audio_play_pause_indicator, is_playing); // Escreve no "LcmVar" "AudioPlayPauseIndicator" o valor da vari�vel "isPlaying"
         lcm_var_write(&audio_next_indicator, next_disabled); // Escreve no "LcmVar" "AudioNextIndicator" o valor da vari�vel "nextDisabled"
         lcm_var_write(&audio_stop_indicator, is_stopped); // Escreve no "LcmVar" "AudioStopIndicator" o valor da vari�vel "isStopped"
         lcm_var_write(&audio_volume, volume); // Escreve no "LcmVar" "AudioVolume" o valor da vari�vel "volume"
         lcm_var_write(&audio_last_track_indicator, QUANTITY_OF_TRACKS); // Escreve no "LcmVar" "AudioLastTrackIndicator" o valor da vari�vel "quantityOfTracks"
         lcm_var_write(&audio_current_track_indicator, current_track); // Escreve no "LcmVar" "AudioCurrentTrackIndicator" o valor da vari�vel "currentTrack"
         lcm_var_write(&pressure_alert_indicator, 0); // Escreve no "LcmVar" "PressureAlertIndicator" o valor "0"
      }
   }


   /*
   * T�rmino DO c�digo da tela de introdu��o (PicId 0 - Intro)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_10_main()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela principal (PicId 10 - main)
   */
   IF(lcm_var_available(&navigation_buttons))      // Verifica se chegou algo no "LcmVar" "NavigationButtons"
   {

      uint16_t value = lcm_var_get_data(&navigation_buttons);    // Atribui o valor recebido a vari�vel "value"

      SWITCH(value)          // Verifica qual dos "Return Values" dos bot�es de navega��o que foi atribuido a vari�vel "value"
      {
            case NAVIGATE_TO_AUDIO:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToAudio"
         case NAVIGATE_TO_RTC:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToRtc"
         case NAVIGATE_TO_CURVE_A0:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToCurveA0"
         case NAVIGATE_TO_CURVE_SIMULATION:      // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToCurveSimulation"
         case NAVIGATE_TO_PRESSURE:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToPressure"
         case NAVIGATE_TO_LED:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToLed"
         case NAVIGATE_TO_TEXT:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToText"
         case NAVIGATE_TO_SETTINGS:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToSettings"
         case NAVIGATE_FROM_VOLUME:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateFromVolume"
         {
            INT picid = lcm_read_pic_id();    // Verifica qual o PicId atual e atribui a vari�vel "picId"
            lcm_var_write(&pic_id_indicator, picid);      // Escreve no "LcmVar" "PicIdIndicator" o valor da vari�vel "picId"
         }

         BREAK;

         case NAVIGATE_TO_VOLUME:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "navigateToVolume"
            IF(is_muted == false)        // Verifica se o mudo n�o foi ativado
            {
               lcm_change_pic_id(pic_id_volume);     // Altera a tela para a tela de volume (PicId 9 - Volume)
               INT picid = lcm_read_pic_id();   // Verifica qual o PicId atual e atribui a vari�vel "picId"
               lcm_var_write(&pic_id_indicator, picid);     // Escreve no "LcmVar" "PicIdIndicator" o valor da vari�vel "picId"
            }

            BREAK;

         DEFAULT:
            BREAK;
      }
   }

   /*
   * T�rmino DO c�digo da tela principal (PicId 10 - main)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_1_audio()
{
   /*******************************************************************************************************************************************
   * In�cio do c�digo da tela de �udio (PicId 1 - Audio)
   */
   IF(lcm_var_available(&audio_controls))       // Verifica se chegou algo no "LcmVar" "AudioControls"
   {
      INT value = lcm_var_get_data(&audio_controls);     // Atribui o valor recebido a vari�vel "value"
      SWITCH(value)          // Verifica qual dos "Return Values" dos bot�es de controle do �udio que foi atribuido a vari�vel "value"
      {
         case AUDIO_CONTROLS_MUTE:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "audioControlsMute"
            IF(is_muted == true)        // Verifica se a vari�vel "isMuted" � verdadeira
            {
               is_muted = false;        // A vari�vel "inMuted" se torna falso
               lcm_var_write(&audio_volume, volume);     // Escreve no "LcmVar" "AudioVolume" o valor da vari�vel "volume"
            }

            ELSE
            {
               is_muted = true;        // A vari�vel "inMuted" se torna verdadeira
               lcm_var_write(&audio_volume, muted_volume);    // Escreve no "LcmVar" "AudioVolume" o valor da vari�vel "mutedVolume"
            }

            BREAK;

         case AUDIO_CONTROLS_REPEAT:       // Caso o valor da vari�vel "value" seja o "RETURN Value" "audioControlsRepeat"
            previous_disabled = false;       // A vari�vel "previousDisabled" se torna falso
            next_disabled = false;        // A vari�vel "nextDisabled" se torna falso
            IF(is_repeating == false)       // Verifica se a vari�vel "isRepeating" � falsa
            {
               is_repeating = true;       // A vari�vel "isRepeating" se torna verdadeira
            }

            ELSE
            {
               is_repeating = false;       // A vari�vel "isRepeating" se torna falso
               IF(current_track == QUANTITY_OF_TRACKS)   // Verifica se a vari�vel "currentTrack" � igual a "quantityOfTracks"
               {
                 next_disabled = true;      // A vari�vel "nextDisabled" se torna verdadeira
               }

               else IF(current_track == 1)     // Verifica se a vari�vel "currentTrack" � igual a 1
               {
                 previous_disabled = true;     // A vari�vel "previousDisabled" se torna verdadeira
               }
            }

            lcm_var_write(&audio_repeat_indicator, is_repeating);   // Escreve no "LcmVar" "AudioRepeatIndicator" o valor da vari�vel "isRepeating"
            lcm_var_write(&audio_previous_indicator, previous_disabled); // Escreve no "LcmVar" "AudioPreviousIndicator" o valor da vari�vel "previousDisabled"
            lcm_var_write(&audio_next_indicator, next_disabled);   // Escreve no "LcmVar" "AudioNextIndicator" o valor da vari�vel "nextDisabled"
            BREAK;

         case AUDIO_CONTROLS_PREVIOUS:       // Caso o valor da vari�vel "value" seja o "RETURN Value" "audioControlsPrevious"
            IF(previous_disabled == false)     // Verifica se a vari�vel "previousDisabled" � falsa
            {
               current_track--;        // Decrementa 1 da vari�vel "currentTrack"
               IF(is_repeating == true)      // Verifica se a vari�vel "previousDisabled" � verdadeira
               {
                  IF(current_track < 1)     // Verifica se a vari�vel "currenttrack" � menor que 1
                  {
                     current_track = QUANTITY_OF_TRACKS;  // Atribui o valor da vari�vel "quantityOfTracks" a vari�vel "currentTrack"
                  }
               }

               ELSE
               {
                  IF(current_track == 1)       // verifica se a vari�vel "currentTrack" � igual a 1
                  {
                    previous_disabled = true;      // A vari�vel "previousDisabled" se torna verdadeira
                    lcm_var_write(&audio_previous_indicator, previous_disabled); // Escreve no "LcmVar" "AudioPreviousIndicator" o valor da vari�vel "previousDisabled"
                  }
               }

               lcm_var_write(&audio_current_track_indicator, current_track); // Escreve no "LcmVar" "AudioCurrentTrackIndicator" o valor da vari�vel "currentTrack"
            }

            IF(next_disabled == true)       // Verifica se a vari�vel "nextDisabled" � verdadeira
            {
               next_disabled = false;       // A vari�vel "nextDisabled" se torna falso
               lcm_var_write(&audio_next_indicator, next_disabled);  // Escreve no "LcmVar" "AudioNextIndicator" o valor da vari�vel "nextDisabled"
            }

            BREAK;

         case AUDIO_CONTROLS_PLAY_PAUSE:       // Caso o valor da vari�vel "value" seja o "RETURN Value" "audioControlsPlayPause"
            IF(is_playing == false)       // Verifica se a vari�vel "isPlaying" � falsa
            {
               is_playing = true;        // A vari�vel "inMuted" se torna verdadeira
               is_stopped = false;       // A vari�vel "isStopped" se torna falso
            }

            ELSE
            {
               is_playing = false;       // A vari�vel "isPlaying" se torna falso
            }

            lcm_var_write(&audio_play_pause_indicator, is_playing);   // Escreve no "LcmVar" "AudioPlayPauseIndicator" o valor da vari�vel "isPlaying"
            lcm_var_write(&audio_stop_indicator, is_stopped);    // Escreve no "LcmVar" "AudioStopIndicator" o valor da vari�vel "isStopped"
            BREAK;

         case AUDIO_CONTROLS_NEXT:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "audioControlsNext"
            IF(next_disabled == false)      // Verifica se a vari�vel "nextDisabled" � falsa
            {
               current_track++;        // Incrementa 1 na vari�vel "currentTrack"
               IF(is_repeating == true)      // Verifica se a vari�vel "isRepeating" � verdadeira
               {
                  IF(current_track > QUANTITY_OF_TRACKS)  // Verifica se a vari�vel "currentTrack" � menor que a vari�vel "quantityOfTracks"
                  {
                     current_track = 1;      // Atribui 1 a vari�vel "currentTrack"
                  }
               }

               ELSE
               {
                  IF(current_track == QUANTITY_OF_TRACKS)  // Verifica se a vari�vel "currentTrack" � igual a vari�vel "quantityOfTracks"
                  {
                    next_disabled = true;     // A vari�vel "nextDisabled" se torna verdadeira
                    lcm_var_write(&audio_next_indicator, next_disabled); // Escreve no "LcmVar" "AudioNextIndicator" o valor da vari�vel "nextDisabled"
                  }
               }

               lcm_var_write(&audio_current_track_indicator, current_track); // Escreve no "LcmVar" "AudioCurrentTrackIndicator" o valor da vari�vel "currentTrack"
            }

            IF(previous_disabled == true)      // Verifica se a vari�vel "previousDisabled" � verdadeira
            {
               previous_disabled = false;      // A vari�vel "previousDisabled" se torna falso
               lcm_var_write(&audio_previous_indicator, previous_disabled); // Escreve no "LcmVar" "AudioPreviousIndicator" o valor da vari�vel "previousDisabled"
            }

            BREAK;

         case AUDIO_CONTROLS_STOP:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "audioControlsStop"
            IF(is_stopped == false)
            {
               is_stopped = true;        // A vari�vel "isStopped" se torna verdadeira
               is_playing = false;       // A vari�vel "isPlaying" se torna falso
            }

            lcm_var_write(&audio_play_pause_indicator, is_playing);   // Escreve no "LcmVar" "AudioPlayPauseIndicator" o valor da vari�vel "isPlaying"
            lcm_var_write(&audio_stop_indicator, is_stopped);    // Escreve no "LcmVar" "AudioStopIndicator" o valor da vari�vel "isStopped"
            BREAK;

         DEFAULT:
            BREAK;
      }
   }

   /*
   * T�rmino DO c�digo da tela principal (PicId 10 - main)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_9_volume()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de volume (PicId 9 - Volume)
   */
   IF(lcm_var_available(&audio_volume))         // Verifica se chegou algo no "LcmVar" "AudioVolume"
   {
      volume = lcm_var_get_data(&audio_volume);       // Atribui o valor recebido DO "LcmVar" "AudioVolume" a vari�vel "volume"
   }

   /*
   * T�rmino DO c�digo da tela de volume (PicId 9 - Volume)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_2_rtc()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de RTC (PicId 2 - RTC)
   */
   IF(lcm_var_available(&rtc_buttons))         // Verifica se chegou algo no "LcmVar" "RtcButtons"
   {

      lcm_read_rtc();

      INT value = lcm_var_get_data(&rtc_buttons);       // Atribui o valor recebido a vari�vel "value"

      keyboard_is_open = true;         // A vari�vel "keyboardIsOpen" se torna verdadeira

      last_keyboard_response_time = millis + keyboard_is_open_read_interval_in_ms; // Atribui o valor de "millis () " somado ao valor da vari�vel "keyboardIsOpenReadIntervalInMs" a vari�vel "lastKeyboardResponseTime"

      current_rtc_keyboard = rtc_keyboard_sequence_none;    // A vari�vel "currentRtcKeyboard" recebe o valor da vari�vel "rtcKeyboardSequenceNone"

      SWITCH(value)           // Verifica qual dos "Return Values" dos bot�es de configura��o do RTC que foi atribuido a vari�vel "value"
      {
         case RTC_BUTTON_DAY:          // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonDay"
            lcm_activate_software_control(software_control_code_day); // Ativa o teclado de configura��o DO dia do RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_day);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconDay"
            BREAK;

         case RTC_BUTTON_MONTH:          // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonMonth"
            lcm_activate_software_control(software_control_code_month); // Ativa o teclado de configura��o DO m�s do RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_month);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconMonth"
            BREAK;

         case RTC_BUTTON_YEAR:          // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonYear"
            lcm_activate_software_control(software_control_code_year); // Ativa o teclado de configura��o DO ano do RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_year);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconYear"
            BREAK;

         case RTC_BUTTON_HOUR:          // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonHour"
            lcm_activate_software_control(software_control_code_hour); // Ativa o teclado de configura��o da hora DO RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_hour);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconHour"
            BREAK;

         case RTC_BUTTON_MINUTE:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonMinute"
            lcm_activate_software_control(software_control_code_minute); // Ativa o teclado de configura��o DO minuto do RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_minute);  // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconMinute"
            BREAK;

         case RTC_BUTTON_SECOND:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonSecond"
            lcm_activate_software_control(software_control_code_second); // Ativa o teclado de configura��o DO segundo do RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_second);  // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconSecond"
            BREAK;

         case RTC_BUTTON_DATE:          // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonDate"
            lcm_activate_software_control(software_control_code_day); // Ativa o teclado de configura��o DO dia do RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_day);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconDay"
            current_rtc_keyboard = rtc_keyboard_sequence_day;   // Atribui o valor da vari�vel "rtcKeyboardSequenceDay" a vari�vel "currentRtcKeyboard"
            BREAK;

         case RTC_BUTTON_TIME:          // Caso o valor da vari�vel "value" seja o "RETURN Value" "rtcButtonTime"
            lcm_activate_software_control(software_control_code_hour); // Ativa o teclado de configura��o da hora DO RTC
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_hour);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconHour"
            current_rtc_keyboard = rtc_keyboard_sequence_hour;   // Atribui o valor davari�vel "rtcKeyboardSequenceHour" a vari�vel "currentRtcKeyboard"
            BREAK;

         DEFAULT:
            BREAK;
      }
   }

   IF(lcm_var_available(&keyboard_day))         // Verifica se chegou algo no "LcmVar" "KeyboardDay"
   {
      last_rtc_day = lcm_var_get_data(&keyboard_day);      // Atribui o valor recebido pelo "LcmVar" "KeyboardDay" a vari�vel "lastRtcDay"
      IF(current_rtc_keyboard == rtc_keyboard_sequence_day)   // Verifica se a vari�vel "currentRtcKeyboard" � igual a vari�vel "rtcKeyboardSequenceDay"
      {
         lcm_activate_software_control(software_control_code_month); // Ativa o teclado de configura��o DO m�s do RTC
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_month);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconMonth"
         current_rtc_keyboard = rtc_keyboard_sequence_month;  // Atribui o valor davari�vel "rtcKeyboardSequenceMonth" a vari�vel "currentRtcKeyboard"
      }

      ELSE
      {
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
         lcm_rtc_write_day(last_rtc_day);       // Atribui o valor da vari�vel "lastRtcDay" no valor DO dia no RTC do display
      }
   }

   IF(lcm_var_available(&keyboard_month))        // Verifica se chegou algo no "LcmVar" "KeyboardMonth"
   {
      last_rtc_month = lcm_var_get_data(&keyboard_month);     // Atribui o valor recebido pelo "LcmVar" "KeyboardMonth" a vari�vel "lastRtcMonth"
      IF(current_rtc_keyboard == rtc_keyboard_sequence_month)  // Verifica se a vari�vel "currentRtcKeyboard" � igual a vari�vel "rtcKeyboardSequenceMonth"
      {
         lcm_activate_software_control(software_control_code_year); // Ativa o teclado de configura��o DO ano do RTC
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_year);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconYear"
         current_rtc_keyboard = rtc_keyboard_sequence_year;   // Atribui o valor davari�vel "rtcKeyboardSequenceYear" a vari�vel "currentRtcKeyboard"
      }

      ELSE
      {
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
         lcm_rtc_write_month(last_rtc_month);      // Atribui o valor da vari�vel "lastRtcMonth" no valor DO m�s no RTC do display
      }
   }

   IF(lcm_var_available(&keyboard_year))         // Verifica se chegou algo no "LcmVar" KeyboardYear"
   {
      last_rtc_year = lcm_var_get_data(&keyboard_year);      // Atribui o valor recebido pelo "LcmVar" "KeyboardYear" a vari�vel "lastRtcYear"
      IF(current_rtc_keyboard == rtc_keyboard_sequence_year)  // Verifica se a vari�vel "currentRtcKeyboard" � igual a vari�vel "rtcKeyboardSequenceYear"
      {
         lcm_rtc_write_date(last_rtc_day, last_rtc_month, last_rtc_year); // Atribui os valores das vari�veis "lastRtcDay", "lastRtcMonth" e "lastRtcYear" nos valores de dia, m�s e ano no RTC DO display
         current_rtc_keyboard = rtc_keyboard_sequence_none;   // Atribui o valor davari�vel "rtcKeyboardSequenceNone" a vari�vel "currentRtcKeyboard"
      }

      ELSE
      {
         lcm_rtc_write_year(last_rtc_year);      // Atribui o valor da vari�vel "lastRtcYear" no valor DO ano no RTC do display
      }

      lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);    // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
   }

   IF(lcm_var_available(&keyboard_hour))         // Verifica se chegou algo no "LcmVar" KeyboardHour"
   {
      last_rtc_hour = lcm_var_get_data(&keyboard_hour);      // Atribui o valor recebido pelo "LcmVar" "KeyboardHour" a vari�vel "lastRtcHour"
      IF(current_rtc_keyboard == rtc_keyboard_sequence_hour)  // Verifica se a vari�vel "currentRtcKeyboard" � igual a vari�vel "rtcKeyboardSequenceHour"
      {
         lcm_activate_software_control(software_control_code_minute); // Ativa o teclado de configura��o DO minuto do RTC
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_minute);  // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconMinute"
         current_rtc_keyboard = rtc_keyboard_sequence_minute;  // Atribui o valor davari�vel "rtcKeyboardSequenceMinute" a vari�vel "currentRtcKeyboard"
      }

      ELSE
      {
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
         lcm_rtc_write_hours(last_rtc_hour);      // Atribui o valor da vari�vel "lastRtcHour" no valor da hora no RTC DO display
      }
   }

   IF(lcm_var_available(&keyboard_minute))        // Verifica se chegou algo no "LcmVar" KeyboardMinute"
   {
      last_rtc_minute = lcm_var_get_data(&keyboard_minute);     // Atribui o valor recebido pelo "LcmVar" "KeyboardMinute" a vari�vel "lastRtcMinute"
      IF(current_rtc_keyboard == rtc_keyboard_sequence_minute)  // Verifica se a vari�vel "currentRtcKeyboard" � igual a vari�vel "rtcKeyboardSequenceMinute"
      {
         lcm_activate_software_control(software_control_code_second); // Ativa o teclado de configura��o DO segundo do RTC
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_second);  // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconSecond"
         current_rtc_keyboard = rtc_keyboard_sequence_second;  // Atribui o valor da vari�vel "rtcKeyboardSequenceSecond" a vari�vel "currentRtcKeyboard"
      }

      ELSE
      {
         lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);   // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
         lcm_rtc_write_minutes(last_rtc_minute);     // Atribui o valor da vari�vel "lastRtcMinute" no valor DO minuto no RTC do display
      }
   }

   IF(lcm_var_available(&keyboard_second))        // Verifica se chegou algo no "LcmVar" KeyboardSecond"
   {
      last_rtc_second = lcm_var_get_data(&keyboard_second);     // Atribui o valor recebido pelo "LcmVar" "KeyboardSecond" a vari�vel "lastRtcSecond"
      IF(current_rtc_keyboard == rtc_keyboard_sequence_second)  // Verifica se a vari�vel "currentRtcKeyboard" � igual a vari�vel "rtcKeyboardSequenceSecond"
      {
         lcm_rtc_write_time(last_rtc_hour, last_rtc_minute, 0); // Atribui os valores das vari�veis "lastRtcHour", "lastRtcMinute" e "lastRtcSecond" nos valores de hora, minuto e segundo no RTC DO display
         current_rtc_keyboard = rtc_keyboard_sequence_none;   // Atribui o valor da vari�vel "rtcKeyboardSequenceNone" a vari�vel "currentRtcKeyboard"
      }

      ELSE
      {
         lcm_rtc_write_seconds(last_rtc_second);     // Atribui o valor da vari�vel "lastRtcSecond" no valor DO segundo no RTC do display
      }

      lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);    // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
   }

   IF(keyboard_is_open == true)         // Verifica se a vari�vel "keyboardIsOpen" � verdadeira
   {
      IF(last_keyboard_response_time < millis)     // Verifica se a vari�vel "lastKeyboardResponseTime" � menor que "millis () "
      {
         keyboard_is_open = lcm_read_keyboard_status();   // Atribui o estado atual DO teclado a vari�vel "keyboardIsOpen"                   


         IF(keyboard_is_open == false)       // Verifica se a vari�vel "keyboardIsOpen" � falsa
         {
            lcm_var_write(&rtc_selected_icons, rtc_selected_icon_none);  // Escreve no "LcmVar" "RtcSelectedIcons" o valor da vari�vel "rtcSelectedIconNone"
         }

         last_keyboard_response_time = millis + keyboard_is_open_read_interval_in_ms; // Atribui o valor de "millis () " somado ao valor da vari�vel "keyboardIsOpenReadIntervalInMs" a vari�vel "lastKeyboardResponseTime"
      }
   }

   /*
   * T�rmino DO c�digo da tela de RTC (PicId 2 - RTC)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_3_adc_curve()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de curva A0 (PicId 3 - CurveA0)
   */
   IF(lcm_var_available(&curve_a0_controls))        // Verifica se chegou algo no "LcmVar" "CurveA0Controls"
   {
      INT value = lcm_var_get_data(&curve_a0_controls);     // Atribui o valor recebido pelo "LcmVar" "CurveA0Controls" a vari�vel "value"
      SWITCH(value)           // Verifica qual dos "Return Values" dos controles da tela curva A0 (PicId 3 - CurveA0) que foi atribuido a vari�vel "value"
      {
         case CURVE_A0_CONTROL_PLAY:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "curveA0ControlPlay"
            curve_a0_is_plotting = true;        // A vari�vel "curveA0isPlotting" se torna verdadeira
            BREAK;

         case CURVE_A0_CONTROL_PAUSE:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "curveA0ControlPause"
            curve_a0_is_plotting = false;       // A vari�vel "curveA0isPlotting" se torna falso
            BREAK;

         case CURVE_A0_CONTROL_STOP:         // Caso o valor da vari�vel "value" seja o "RETURN Value" "curveA0ControlStop"
            lcm_clear_trend_curve0();        // Apaga todos os pontos DO "Trend Curve Display" do canal "0"
            curve_a0_is_plotting = false;       // A vari�vel "curveA0isPlotting" se torna falso
            BREAK;

         DEFAULT:
            BREAK;
      }
   }

   IF(curve_a0_is_plotting == true)        // Verifica se a vari�vel "curveA0isPlotting" � verdadeira
   {
      IF(last_curve_a0_write_time < millis)      // Verifica se a vari�vel "lastCurveA0WriteTime" � menor que "millis () "
      {
         set_adc_channel(0);
         uint32_t value = read_adc();

         value = value * 100;

         value = value / 65535;      // C�lculo para fazer o valor de value ficar entre 0 e 100

         lcm_write_trend_curve0(value);       // Envia o valor da vari�vel "value" para o canal 0 DO "trend Curve Display"

         lcm_var_write(&curve_a0_temperature_indicator, (uint16_t)value);  // Escreve no "LcmVar" "CurveA0TemperatureIndicator" o valor da vari�vel "value"

         last_curve_a0_write_time = millis + curve_a0_write_interval_in_ms; // Atribui o valor de "millis () " somado ao valor da vari�vel "curveA0WriteIntervalInMs" a vari�vel "lastCurveA0WriteTime"
      }
   }

   /*
   * T�rmino DO c�digo da tela de curva A0 (PicId 3 - CurveA0)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_4_curve_simulation()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de simula��o da curva (PicId 4 - CurveSim)
   */
   IF(lcm_var_available(&curve_simulation_sample_buttons))     // Verifica se chegou algo no "LcmVar" "CurveSimulationSampleButtons"
   {
      INT value = lcm_var_get_data(&curve_simulation_sample_buttons);  // Atribui o valor recebido pelo "LcmVar" "CurveSimulationSampleButtons" a vari�vel "value"
      SWITCH(value)           // Verifica qual dos "Return Values" dos bot�es de simula��o de curva foi atribuido a vari�vel "value"
      {
         case CURVE_SIMULATION_SAMPLE_ECG:       // Caso o valor da vari�vel "value" seja o "RETURN Value" "curveSimulationSampleEcg"
         {
            ecg_sample_activated++;
         }

         BREAK;

         case CURVE_SIMULATION_SAMPLE_SINE:      // Caso o valor da vari�vel "value" seja o "RETURN Value" "curveSimulationSampleSine"
         {
            sine_sample_activated++;
         }

         BREAK;

      DEFAULT:
         BREAK;
      }
   }

   if (ecg_sample_activated > 0)
   {
      if (ecg_curve_current_point <= curve_number_of_points)
      {
         lcm_write_trend_curve1(ecg_sample_points[ecg_sample_index]); // Envia o valor da vari�vel "ecgSamplePoints[ecgSampleIndex]" para o canal 1 DO "trend Curve Display"

         lcm_var_write(&curve_simulation_temperature_indicator, ecg_sample_points[ecg_sample_index]); // Escreve no "LcmVar" "CurveSimulationTemperatureIndicator" o valor da vari�vel "ecgSamplePoints[ecgSampleIndex]"

         lcm_var_write(&curve_simulation_manual_input, ecg_sample_points[ecg_sample_index]); // Escreve no "LcmVar" "CurveSimulationManualInput" o valor da vari�vel "ecgSamplePoints[ecgSampleIndex]"

         ecg_sample_index++;

         delay_ms(10);

         IF(ecg_sample_index == ecg_sample_points_length)   // Verifica se a vari�vel "ecgSampleIndex" � igual a vai�vel "ecgSamplePointsLength"
         {
            ecg_sample_index = 0;         // Atribui o valor "0" a variavel "ecgSampleIndex"
         }

         ecg_curve_current_point++;
      }
      else
      {
         ecg_sample_activated--;
         ecg_curve_current_point = 0;
      }
   }
   else
   {
      if (sine_sample_activated > 0)
      {
         if (sine_curve_current_point <= curve_number_of_points)
         {
            lcm_write_trend_curve1(sine_sample_points[sine_sample_index]); // Envia o valor da vari�vel "ecgSamplePoints[ecgSampleIndex]" para o canal 1 DO "trend Curve Display"

            lcm_var_write(&curve_simulation_temperature_indicator, sine_sample_points[sine_sample_index]); // Escreve no "LcmVar" "CurveSimulationTemperatureIndicator" o valor da vari�vel "ecgSamplePoints[ecgSampleIndex]"

            lcm_var_write(&curve_simulation_manual_input, sine_sample_points[sine_sample_index]); // Escreve no "LcmVar" "CurveSimulationManualInput" o valor da vari�vel "ecgSamplePoints[ecgSampleIndex]"

            sine_sample_index++;

            delay_ms(10);

            IF(sine_sample_index == sine_sample_points_length)   // Verifica se a vari�vel "ecgSampleIndex" � igual a vai�vel "ecgSamplePointsLength"
            {
               sine_sample_index = 0;         // Atribui o valor "0" a variavel "ecgSampleIndex"
            }

            sine_curve_current_point++;
         }
         else
         {
            sine_sample_activated--;
            sine_curve_current_point = 0;
         }
      }
   }

   IF(lcm_var_available(&curve_simulation_manual_input))     // Verifica se chegou algo no "LcmVar" "CurveSimulationManualInput"
   {
      INT value = lcm_var_get_data(&curve_simulation_manual_input);   // Atribui o valor recebido pelo "LcmVar" "CurveSimulationManualInput" a vari�vel "value"
      lcm_write_trend_curve1(value);      // Envia o valor da vari�vel "value" para o canal 1 DO "trend Curve Display"
      lcm_var_write(&curve_simulation_temperature_indicator, value);   // Escreve no "LcmVar" "CurveSimulationTemperatureIndicator" o valor da vari�vel "value"
   }

   /*
   * T�rmino DO c�digo da tela de simula��o da curva (PicId 4 - CurveSim)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_5_pressure()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de press�o (PicId 5 - Pressure)
   */
   IF(lcm_var_available(&pressure_setpoint))        // Verifica se chegou algo no "LcmVar" "PressureSetpoint"
   {
      adjust_pressure = true;         // A vari�vel "adjustPressure" se torna verdadeira
      pressure_setpoint_value = lcm_var_get_data(&pressure_setpoint);  // Atribui o valor recebido pelo "LcmVar" "PressureSetpoint" a vari�vel "pressureSetpointValue"
   }

   IF(adjust_pressure == true)         // Verifica se a vari�vel "adjustPressure" � verdadeira
   {
      IF(last_pressure_write_time < millis)      // Verifica se a vari�vel "lastPressureWriteTime" � menor que "millis () "
      {
         IF(pressure_setpoint_value > current_pressure_value)  // Verifica se a vari�vel "pressureSetpointValue" � maior que a vari�vel"currentPressureValue"
         {
            current_pressure_value++;       // Incrementa "1" a vari�vel "currentPressureValue"
         }

         else IF(pressure_setpoint_value < current_pressure_value) // Verifica se a vari�vel "pressureSetpointValue" � menor que a vari�vel"currentPressureValue"
         {
         current_pressure_value--;       // Decrementa "1" da vari�vel "currentPressureValue"
         }

         IF(pressure_setpoint_value == current_pressure_value) // Verifica se a vari�vel "pressureSetpointValue" � igual a vari�vel "currentPressureValue"
         {
            adjust_pressure = false;       // A vari�vel "adjustPressure" se torna falso
         }

         IF(current_pressure_value >= pressure_alert_level)  // Verifica se a vari�vel "currentPressureValue" � maior ou igual a vari�vel "pressureAlertLevel"
         {
            lcm_var_write(&pressure_alert_indicator, 1);     // Escreve no "LcmVar" "PressureAlertIndicator" o valor da vari�vel "1"
         }

         ELSE
         {
            lcm_var_write(&pressure_alert_indicator, 0);     // Escreve no "LcmVar" "PressureAlertIndicator" o valor da vari�vel "0"
         }

         lcm_var_write(&current_pressure_indicator, current_pressure_value); // Escreve no "LcmVar" "CurrentPressureIndicator" o valor da vari�vel "currentPressureValue"
         last_pressure_write_time = millis + pressure_write_interval_in_ms; // Atribui o valor de "millis () " somado ao valor da vari�vel "pressureWriteIntervalInMs" a vari�vel "lastPressureWriteTime"
      }
   }

   /*
   * T�rmino DO c�digo da tela de press�o (PicId 5 - Pressure)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_6_led()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de LED (PicId 6 - Led)
   */
   IF(lcm_var_available(&led_controls))         // Verifica se chegou algo no "LcmVar" "LedControls"
   {
      uint16_t value = lcm_var_get_data(&led_controls);      // Atribui o valor recebido a vari�vel "value"

      SWITCH(value)           // Verifica qual dos "Return Values" dos bot�es de controle de led foi atribuido a vari�vel "value"
      {
         case LED_CONTROLS_TOGGLE_LED1:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsToggleLed1"
            lcm_var_request(&led1_indicator);        // Pede para o "LcmVar" "Led1Indicator" o seu valor
            BREAK;

         case LED_CONTROLS_TOGGLE_LED2:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsToggleLed2"
            lcm_var_request(&led2_indicator);        // Pede para o "LcmVar" "Led2Indicator" o seu valor
            BREAK;

         case LED_CONTROLS_TOGGLE_LED3:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsToggleLed3"
            lcm_var_request(&led3_indicator);        // Pede para o "LcmVar" "Led3Indicator" o seu valor
            BREAK;

         case LED_CONTROLS_TOGGLE_LED4:        // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsToggleLed4"
            lcm_var_request(&led4_indicator);        // Pede para o "LcmVar" "Led4Indicator" o seu valor
            BREAK;

         case LED_CONTROLS_SET_ALL_LEDS_TO_LOW:     // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsSetAllLedsToLow"
            lcm_var_write(&led1_indicator, LOW);        // Escreve no "LcmVar" "Led1Indicator" o valor "LOW" - Desligado
            lcm_var_write(&led2_indicator, LOW);        // Escreve no "LcmVar" "Led2Indicator" o valor "LOW" - Desligado
            lcm_var_write(&led3_indicator, LOW);        // Escreve no "LcmVar" "Led3Indicator" o valor "LOW" - Desligado
            lcm_var_write(&led4_indicator, LOW);        // Escreve no "LcmVar" "Led4Indicator" o valor "LOW" - Desligado
            digital_write(LED_PIN1, LOW);       // Escreve no pino "ledPin1" o valor "LOW" - Desligado
            digital_write(LED_PIN2, LOW);       // Escreve no pino "ledPin2" o valor "LOW" - Desligado
            digital_write(LED_PIN3, LOW);       // Escreve no pino "ledPin3" o valor "LOW" - Desligado
            digital_write(LED_PIN4, LOW);       // Escreve no pino "ledPin4" o valor "LOW" - Desligado
            BREAK;

         case LED_CONTROLS_TOGGLE_ALL_LEDS:       // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsToggleAllLeds"
            lcm_var_request(&led1_indicator);
            lcm_var_request(&led2_indicator);
            lcm_var_request(&led3_indicator);
            lcm_var_request(&led4_indicator);
            BREAK;

         case LED_CONTROLS_SET_ALL_LEDS_TO_HIGH:      // Caso o valor da vari�vel "value" seja o "RETURN Value" "ledControlsSetAllLedsToHigh"
            lcm_var_write(&led1_indicator, HIGH);       // Escreve no "LcmVar" "Led1Indicator" o valor "HIGH" - Ligado
            lcm_var_write(&led2_indicator, HIGH);       // Escreve no "LcmVar" "Led2Indicator" o valor "HIGH" - Ligado
            lcm_var_write(&led3_indicator, HIGH);       // Escreve no "LcmVar" "Led3Indicator" o valor "HIGH" - Ligado
            lcm_var_write(&led4_indicator, HIGH);       // Escreve no "LcmVar" "Led4Indicator" o valor "HIGH" - Ligado
            digital_write(LED_PIN1, HIGH);       // Escreve no pino "ledPin1" o valor "HIGH" - Ligado
            digital_write(LED_PIN2, HIGH);       // Escreve no pino "ledPin2" o valor "HIGH" - Ligado
            digital_write(LED_PIN3, HIGH);       // Escreve no pino "ledPin3" o valor "HIGH" - Ligado
            digital_write(LED_PIN4, HIGH);       // Escreve no pino "ledPin4" o valor "HIGH" - Ligado
            BREAK;

         DEFAULT:
            BREAK;
      }
   }

   IF(lcm_var_available(&led1_indicator))       // Verifica se chegou algo no "LcmVar" "Led1Indicator"
   {
      INT value = lcm_var_get_data(&led1_indicator);     // Atribui o valor recebido a vari�vel "value"
      value = !value;          // Inverte o valor da variavel "value"
      lcm_var_write(&led1_indicator, value);       // Escreve no "LcmVar" "Led1Indicator" o valor da vari�vel "value"
      digital_write(LED_PIN1, value);       // Escreve no pino "ledPin1" o valor da vari�vel "value"
   }

   IF(lcm_var_available(&led2_indicator))       // Verifica se chegou algo no "LcmVar" "Led2Indicator"
   {
      INT value = lcm_var_get_data(&led2_indicator);     // Atribui o valor recebido a vari�vel "value"
      value = !value;          // Inverte o valor da variavel "value"
      lcm_var_write(&led2_indicator, value);       // Escreve no "LcmVar" "Led2Indicator" o valor da vari�vel "value"
      digital_write(LED_PIN2, value);       // Escreve no pino "ledPin2" o valor da vari�vel "value"
   }

   IF(lcm_var_available(&led3_indicator))       // Verifica se chegou algo no "LcmVar" "Led3Indicator"
   {
      INT value = lcm_var_get_data(&led3_indicator);     // Atribui o valor recebido a vari�vel "value"
      value = !value;          // Inverte o valor da variavel "value"
      lcm_var_write(&led3_indicator, value);       // Escreve no "LcmVar" "Led3Indicator" o valor da vari�vel "value"
      digital_write(LED_PIN3, value);       // Escreve no pino "ledPin3" o valor da vari�vel "value"
   }

   IF(lcm_var_available(&led4_indicator))       // Verifica se chegou algo no "LcmVar" "Led4Indicator"
   {
      INT value = lcm_var_get_data(&led4_indicator);     // Atribui o valor recebido a vari�vel "value"
      value = !value;          // Inverte o valor da variavel "value"
      lcm_var_write(&led4_indicator, value);       // Escreve no "LcmVar" "Led4Indicator" o valor da vari�vel "value"
      digital_write(LED_PIN4, value);       // Escreve no pino "ledPin4" o valor da vari�vel "value"
   }


   /*
   * T�rmino DO c�digo da tela de LED (PicId 6 - Led)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_7_text()
{
   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de texto (PicId 7 - Text)
   */
   IF(lcm_string_available(&text_input))       // Verifica se chegou algo no "LcmString" "TextInput"
   {
      char characters[TEXT_LENGTH_IN_CHARS];        // Cria uma vari�vel DO tipo "String" com o nome "characters"
      char invertedCharacters[TEXT_LENGTH_IN_CHARS];      // Cria uma vari�vel DO tipo "String" com o nome "invertedCharacters"

      uint8_t character_index = 0;       // Cria uma vari�vel do tipo "int" com o nome "characterIndex" e atribui o valor "0" a ela
      uint8_t inverted_character_index = 0;       // Cria uma vari�vel do tipo "int" com o nome "characterIndex" e atribui o valor "0" a ela

      WHILE(lcm_string_available(&text_input))      // Verifica e executa enquanto existir algo no "LcmString" "TextInput"
      {
         characters[character_index] = lcm_string_get_data(&text_input);  // Concatena o valor recebido pelo "LcmString" "TextInput" a String "characters"
         character_index++;        // Incrementa "1" a variavel "characterIndex"
      }

      uint8_t received_bytes = character_index;
      inverted_character_index = character_index;
      character_index = 0;

      WHILE(inverted_character_index)        // Verifica e executa enquanto o valor da vari�vel "characterIndex" for maior que "0"
      {
         inverted_character_index--;         // Decrementa "1" da vari�vel "characterIndex"

         invertedCharacters[inverted_character_index] = characters[character_index]; // Concatena os caracteres da vari�vel "characters" a String "characters"

         character_index++;         // Decrementa "1" da vari�vel "characterIndex"
      }

      lcm_string_write(&inverted_text, invertedCharacters, received_bytes);   // Escreve no "LcmString" "InvertedText" o valor da vari�vel "invertedCharacters"
   }

   /*
   * T�rmino DO c�digo da tela de texto (PicId 7 - Text)
   *******************************************************************************************************************************************/
}

VOID lcm_picid_8_settings()
{

   /*******************************************************************************************************************************************
   * In�cio DO c�digo da tela de ajustes (PicId 8 - Settings)
   */

   IF(lcm_var_available(&back_light))       // Verifica se chegou algo no "LcmVar" "BackLight"
   {
      INT value = lcm_var_get_data(&back_light);     // Atribui o valor recebido a vari�vel "value"
      lcm_change_brightness(value);      // Altera o valor DO "Backlight" do "lcm" para o valor da vari�vel "value"
   }

   IF(lcm_var_available(&read_version_button))     // Verifica se chegou algo no "LcmVar" "ReadVersionButton"
   {
      lcm_var_get_data(&read_version_button);      // Realiza a leitura DO que chegou no "LcmVar" "ReadVersionButton" mas o valor recebido n�o � usado
      INT value = lcm_read_firmware_version();   // Verifica qual a vers�o do firmware do display e atribui a vari�vel "value"
      value = (value - (6 * (value >> 4)));   // Realiza a convers�o de BCD para inteiro. 0x28 BCD - > 28 inteiro
      lcm_var_write(&version_indicator, value);     // Escreve no "LcmVar" "VersionIndicator" o valor da vari�vel "value"
   }

   IF(lcm_var_available(&activate_buzzer_button))     // Verifica se chegou algo no "LcmVar" "ActivateBuzzerButton"
   {
      lcm_var_get_data(&activate_buzzer_button);     // Realiza a leitura DO que chegou no "LcmVar" "ActivateBuzzerButton" mas o valor recebido n�o � usado
      INT value = lcm_var_read(&buzzer_activation_time);   // Verifica qual o valor que foi configurado do campo "Buzzer" da tela de ajustes (PicId 8 - Settings) e atribui a vari�vel "value"
      lcm_beep_buzzer(value);       // Ativa o buzzer de acordo com o valor da vari�vel "value" (Milissegundo)
   }

   IF(lcm_var_available(&calibrate_button))      // Verifica se chegou algo no "LcmVar" "CalibrateButton"
   {
      INT value = lcm_var_get_data(&calibrate_button);   // Atribui o valor recebido a vari�vel "value"
      IF(value == popup_confirm)      // Verifica se o valor da vari�vel "value" � igual a vari�vel "popupConfirm"
      {
         lcm_start_calibration();        // Inicia o processo de calibra��o DO Display
      }
   }

   IF(lcm_var_available(&reset_button))       // Verifica se chegou algo no "LcmVar" "ResetButton"
   {
      INT value = lcm_var_get_data(&reset_button);    // Atribui o valor recebido a vari�vel "value"
      IF(value == popup_confirm)      // Verifica se o valor da vari�vel "value" � igual a vari�vel "popupConfirm"
      {
         lcm_reset_lcm();        // Reseta o Display
      }
   }

   /*
   * T�rmino DO c�digo da tela de ajustes (PicId 8 - Settings)
   *******************************************************************************************************************************************/
}


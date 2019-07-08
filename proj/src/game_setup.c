// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "game_structs.h"
#include "sprite.h"
#include "sllist.h"
#include "keyboard.h"
#include "player.h"
#include "../res/xpm.h"
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "game_states.h"
#include "video_card.h"
#include "rtc.h"

int ints_init(struct devices_data *io_data) {
  timer_subscribe_int(&io_data->timer_bitmask);
  io_data->keyboard_bitmask=keyboard_subscribe();
  io_data->mouse_bitmask=subscribe_mouse(); 
  io_data->rtc_bitmask=rtc_subscribe();
  rtc_init();
	//Enabling data reporting
  mouse_disable_int();
	if(mouse_write_byte(MOUSE_ENABLE_DATA_REPORTING) != 0){
		printf("mouse_test_packet::Error enabling data reporting!\n");
		return 1;
	}

  mouse_enable_int();
  return 0;
}

void sprites_init(Sprite **background, struct sprites *sprite/*, Sprite** cursor, Sprite **sp, Sprite **bullet, struct sllist **enemies*/) {
  *background = create_sprite(menu_background_xpm,0, 0, 0, 0);
  //*cursor = create_sprite(cursor_xpm,0,0,20,20);
  sprite->cursor = create_sprite(cursor_xpm,0,0,0,0);
  //*sp = create_sprite(spaceship_xpm, 512, 384, 20, 20);
  sprite->player = create_sprite(spaceship_xpm, 512, 600, 20, 20);
  //*bullet = create_sprite(bullet_xpm,0,0,0,-10);
  sprite->bullet = create_sprite(bullet_xpm,0,0,0,-10);
  sprite->enemies = sllist_create();
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 128, 100, -5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 384, 100, 5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 640, 100, -5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 896, 100, 5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 128, 200, 5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 384, 200, 5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 640, 200, -5, 0));
  sllist_push_back(sprite->enemies,create_sprite(enemyship_xpm, 896, 200, 5, 0));
  sprite->score=create_sprite(score_xpm,5,722,0,0);
  sprite->number[0]=create_sprite(zero_xpm,0,0,0,0);
  sprite->number[1]=create_sprite(one_xpm,0,0,0,0);
  sprite->number[2]=create_sprite(two_xpm,0,0,0,0);
  sprite->number[3]=create_sprite(three_xpm,0,0,0,0);
  sprite->number[4]=create_sprite(four_xpm,0,0,0,0);
  sprite->number[5]=create_sprite(five_xpm,0,0,0,0);
  sprite->number[6]=create_sprite(six_xpm,0,0,0,0);
  sprite->number[7]=create_sprite(seven_xpm,0,0,0,0);
  sprite->number[8]=create_sprite(eight_xpm,0,0,0,0);
  sprite->number[9]=create_sprite(nine_xpm,0,0,0,0);
  sprite->slash=create_sprite(slash_xpm,0,0,0,0);
  sprite->colon=create_sprite(colon_xpm,0,0,0,0);
  sprite->explosion[0]=create_sprite(exp1_xpm,0,0,0,0);
  sprite->explosion[1]=create_sprite(exp2_xpm,0,0,0,0);
  sprite->explosion[2]=create_sprite(exp3_xpm,0,0,0,0);
  sprite->explosion[3]=create_sprite(exp4_xpm,0,0,0,0);
  sprite->explosion[4]=create_sprite(exp5_xpm,0,0,0,0);
  sprite->explosion[5]=create_sprite(exp6_xpm,0,0,0,0);
  sprite->explosion[6]=create_sprite(exp7_xpm,0,0,0,0);
  sprite->explosion[7]=create_sprite(exp8_xpm,0,0,0,0);
  sprite->explosion[8]=create_sprite(exp9_xpm,0,0,0,0);
  sprite->player_exp[0]=create_sprite(exp1_xpm,0,0,0,0);
  sprite->player_exp[1]=create_sprite(exp2_xpm,0,0,0,0);
  sprite->player_exp[2]=create_sprite(exp3_xpm,0,0,0,0);
  sprite->player_exp[3]=create_sprite(exp4_xpm,0,0,0,0);
  sprite->player_exp[4]=create_sprite(exp5_xpm,0,0,0,0);
  sprite->player_exp[5]=create_sprite(exp6_xpm,0,0,0,0);
  sprite->player_exp[6]=create_sprite(exp7_xpm,0,0,0,0);
  sprite->player_exp[7]=create_sprite(exp8_xpm,0,0,0,0);
  sprite->player_exp[8]=create_sprite(exp9_xpm,0,0,0,0);
  sprite->enemy_bullet[0]=create_sprite(enemy_bullet_xpm,0,0,0,7);
  sprite->enemy_bullet[1]=create_sprite(enemy_bullet_xpm,0,0,0,7);
  sprite->enemy_bullet[2]=create_sprite(enemy_bullet_xpm,0,0,0,7);
  sprite->hearts[0]=create_sprite(heart_xpm,974,722,0,0);
  sprite->hearts[1]=create_sprite(heart_xpm,914,722,0,0);
  sprite->hearts[2]=create_sprite(heart_xpm,854,722,0,0);
  sprite->gamewon=create_sprite(you_won_xpm,342,80,0,0);
  sprite->gamelost=create_sprite(you_lost_xpm,327,80,0,0);
  return;
}

void menu_init(Sprite *background, Sprite *cursor) {
  draw_sprite(background,0,0);
  draw_sprite(cursor,0,0);
  return;
}

void game_init(Sprite **background, struct game_data *game_data, struct devices_data *io_data){
  sprites_init(background, game_data->sprite);
  menu_init(*background, game_data->sprite->cursor);
  game_data->score=0;
  game_data->bullet_flag=0;
  io_data->timer_int_cnt=0;
  game_data->exp_flag=0;
  game_data->enemy_bullet_flag[0]=0;
  game_data->enemy_bullet_flag[1]=0;
  game_data->enemy_bullet_flag[2]=0;
  game_data->player_lives=3;
  game_data->invulnerability_flag=1;
  game_data->player_exp_flag=0;
  game_data->player_exp_frame_cnt=0;
  srand(time(NULL));
  return;
}

void delete_all_sprites(Sprite **background, struct sprites *sprite){
  destroy_sprite(*background);
  destroy_sprite(sprite->cursor);
  destroy_sprite(sprite->player);
  destroy_sprite(sprite->bullet);
  sllist_destroy(sprite->enemies);

  for(int i = 0; i < 9; i++){
    destroy_sprite(sprite->explosion[i]);
  }
  for(int j = 0; j < 3; j++){
    destroy_sprite(sprite->enemy_bullet[j]);
  }

  return;
}

int ints_unsub(){
  timer_unsubscribe_int();
  keyboard_unsubscribe();

  if(mouse_disable_int() == -1){
    return -1;
  }
  
  //mouse to its original state
  mouse_write_byte(MOUSE_DISABLE_DATA_REPORTING);

  if(mouse_enable_int() == -1){
    return -1;
  }
  //unsubscribing mouse from interrupts notification
  unsubscribe_mouse();

  rtc_unsubscribe();

  
  clean_output_buffer();
  return 0;
}

void game_exit(Sprite **background, struct sprites *sprite){
  ints_unsub();
  delete_all_sprites(background, sprite);
  vg_exit();
  
  return;
}

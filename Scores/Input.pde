

void keyPressed()
{
  // 37 - left, 38 - up, 39 - right, 40 - down
  if (keyCode == 9 && STATE != ZOMBIE_NAME) // TAB
  {
    INPUT_GAME = (INPUT_GAME + 1) % 3;
    println("INPUT IN GAME:" + game_names[INPUT_GAME]);
    enter_time[INPUT_GAME] = false;
    if (INPUT_GAME > 0)
    {
      enter_name[INPUT_GAME] = true;
      for (int c = 0; c < 3; c++)
      {
        cmd_names[INPUT_GAME][c] = '*';
        char_ix[INPUT_GAME][c] = 0;
      }
      cmd_times[INPUT_GAME] = 0;
      char_pos[INPUT_GAME] = 0;
    }
  }
  if (enter_time[INPUT_GAME])
  {
    if (keyCode == 39) cmd_times[INPUT_GAME] = cmd_times[INPUT_GAME] + 10;
    if (keyCode == 37) cmd_times[INPUT_GAME] = max(0, cmd_times[INPUT_GAME] - 10);
    if (keyCode == 38) cmd_times[INPUT_GAME] = cmd_times[INPUT_GAME] + 1;
    if (keyCode == 40) cmd_times[INPUT_GAME] = max(cmd_times[INPUT_GAME] - 1, 0);
    if (keyCode == 10)
    {
      String cmd_name_str = toStr(cmd_names[INPUT_GAME]);
      sendData(cmd_name_str, cmd_times[INPUT_GAME]);
      enter_time[INPUT_GAME] = false;
      cmd_times[INPUT_GAME] = 0;
      for (int c = 0; c < 3; c++) 
      {
        cmd_names[INPUT_GAME][c] = '*';
        char_ix[INPUT_GAME][c] = 0; //index in alpha
      }
      score_loaded = false;
      INPUT_GAME = 0;
    }
  }
  if (enter_name[INPUT_GAME])
  {
    if (keyCode == 37) char_pos[INPUT_GAME] = max(char_pos[INPUT_GAME] - 1, 0);
    if (keyCode == 39) char_pos[INPUT_GAME] = min(char_pos[INPUT_GAME]+1, 2);
    if (keyCode == 40)
    {
      char_ix[INPUT_GAME][char_pos[INPUT_GAME]] = max(0, char_ix[INPUT_GAME][char_pos[INPUT_GAME]]-1);
      cmd_names[INPUT_GAME][char_pos[INPUT_GAME]] = alpha.charAt(char_ix[INPUT_GAME][char_pos[INPUT_GAME]]);
    }
    if (keyCode == 38) 
    {
      char_ix[INPUT_GAME][char_pos[INPUT_GAME]] = min(alpha.length()-1, char_ix[INPUT_GAME][char_pos[INPUT_GAME]]+1);
      cmd_names[INPUT_GAME][char_pos[INPUT_GAME]] = alpha.charAt(char_ix[INPUT_GAME][char_pos[INPUT_GAME]]);
    }
    if (keyCode == 10)
    {
      if (name_ok[INPUT_GAME]) 
      {
        if (STATE != ZOMBIE_NAME) enter_time[INPUT_GAME] = true;
        else sendName(toStr(cmd_names[INPUT_GAME]));
        name_ok[INPUT_GAME] = false;
        enter_name[INPUT_GAME] = false;
        char_pos[INPUT_GAME] = 0;
      } else 
      {
        start_draw_error_rect = millis();
        println("Wrong name, to quit press TAB");
      }
    }
  }
}

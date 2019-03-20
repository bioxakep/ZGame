boolean startGame(long t)
{
  boolean start = false;
  if (t - lastRFIDCheck > 100)
  {
    lastRFIDCheck = t;
    if (startRFWait) startRFWait = !getStartRFID();
  }
  startStates[0] = debounce(startStates[1], startBtn); // READ START BUTTON
  if (!startStates[0] && startStates[1] || !startRFWait) start = true;
  startStates[1] = startStates[0];
  return start;
}

boolean radioGadget()
{
  
}

void Generator()
{

}

void Generator2()
{

}

void Shelf()
{

}

void Shelf2()
{

}

void Head()
{

}

void Fuses()
{

}

void Fuses2()
{

}

void Map()
{

}

void Map2()
{

}

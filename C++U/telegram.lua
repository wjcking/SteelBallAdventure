SEND_IMMEDIATELY = 0.0
MessageType =
{
  sting = -3,
  dead = -1,
  fall = -2,
  hurting = 0,
  touched = 1,
  pushing = 2,
  medicKit = 3, --加血
  ammo = 4,
  openUp = 5,--打开箱子
  knocked =6,
  pushed = 7,
  trigger=8,
  attack=9,
  hit = 10,
  gotShot = 11,
  fuse=12,
  collider=13,
  obb=14,
  notice=15,
}

local Telegram = {}
function Telegram:new(time,s, r, msg,i)
  local telegram =
  {
    dispatchTime = time,--os.date(“%Y-%m-%d %H:%M:%S”)
    sender = s,
    receiver = r,
    message  = msg,
    info = i,
  }
  setmetatable(telegram, {__index = Telegram})  
  return telegram
end
return Telegram
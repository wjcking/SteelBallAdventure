local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State18 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}
local CreditIndex = 2
local fruitGreen   = 0
local fruitRed  = 0
local coin  = 0
local diamond  = 0



State18.initialize = function()
  CreditIndex = 2
  fruitGreen   = 0
  fruitRed  = 0
  coin  = 0
  diamond  = 0
end
local function dropObject(pos)

  local objects = 
  {
    {type=RoleType.robject, step=Vec2(0.5,0.5),zorder=ZOrder.cover-1,pos=pos,frame="fruit_green.png", allowGravity=true,},
    {type=RoleType.robject, step=Vec2(0.5,0.5),zorder=ZOrder.cover-1,pos=pos,frame="fruit_red.png", allowGravity=true,},
    {type=RoleType.robject, step=Vec2(0.5,0.5),zorder=ZOrder.cover-1,pos=pos,frame="coin.png", allowGravity=true,},
    {type=RoleType.robject, step=Vec2(0.5,0.5),zorder=ZOrder.cover-1,pos=pos,frame="diamond.png", allowGravity=true,},
  }
  local iter = 1
  objects[iter].toast = ObjectToasts[iter]
  objects[iter].fsm = Fsm:new(objects[iter],nil,State18.RObject.DropObject) iter=iter+1
  objects[iter].toast = ObjectToasts[iter]
  objects[iter].fsm = Fsm:new(objects[iter],nil,State18.RObject.DropObject) iter=iter+1
  objects[iter].toast = ObjectToasts[iter]
  objects[iter].fsm = Fsm:new(objects[iter],nil,State18.RObject.DropObject) iter=iter+1
  objects[iter].toast = ObjectToasts[iter]
  objects[iter].fsm = Fsm:new(objects[iter],nil,State18.RObject.DropObject) iter=iter+1
  createRole( objects[math.random(#objects)])

end
State18.RObject.DropObject =  State:new()  
State18.RObject.DropObject.execute = function(object) 
  if (not object.ref:inAir(false)) then
    object.ref:setForward()
    if (object.ref:getDistance().x > ScreenWidth) then
      object.isDisposed = true
      clearDisposed() 
    end
  end
end
State18.RObject.DropObject.onMessage = function(object, telegram)
  local function isAchieved(flag,index)
    if flag then
       Ref.stage:promptAchieve(index,PopupInfo.achieve(index))
    end
  end
  object.ref:setAnimation(object.toast)
  if  object.frame =="fruit_green.png" then
    fruitGreen = fruitGreen + 1
    isAchieved(fruitGreen >= 100,AchieveIndex.Healthy)
  elseif  object.frame =="fruit_red.png" then
    fruitRed = fruitRed + 1
    isAchieved(fruitRed >= 100,AchieveIndex.Happiness)
  elseif  object.frame =="coin.png" then
    coin = coin + 1
    isAchieved(coin >= 100,AchieveIndex.Rich)
  elseif  object.frame =="diamond.png" then 
    diamond = diamond + 1
    isAchieved(diamond >= 100,AchieveIndex.Monopoly)
  end
  object.isDisposed = true
  clearDisposed()
end

State18.RObject.Respawn =  State:new()   
State18.RObject.Respawn.execute = function(object) 

  if  object.ref:getTouchTimes() ~= object.touchTimes and 
  object.ref:getPositionX() < Role[1].ref:getPositionX()  and
  object.ref:getPositionX() > Role[1].ref:getPositionX() - 150    then

    dropObject(object.ref:getBoundSelf().tilePosition)
    object.touchTimes =object.ref:getTouchTimes()
  end
  --记录上一次位置
  object.ref:setDirection(object.direction)
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if (object.ref:getDistance().x >= object.distance.x or
    object.ref:getDistance().y >=object.distance.y) then 
    object.ref:resetVelocity()
    object.ref:respawn()
    if (object.tag == RoleIndex.credit and CreditIndex <= #Credit) then
      object.ref:registerText(Credit[CreditIndex])
      CreditIndex = CreditIndex + 1
      if CreditIndex == #Credit  then
         Ref.stage:promptAchieve(AchieveIndex.Final,PopupInfo.achieve(AchieveIndex.Final))
      end
    end
  end
end

State18.RObject.RoadSign =  State:new()  
State18.RObject.RoadSign.execute = function(object) 
--  if (object.direction == MovingDirection.stayStill) then
--    return
--  end
  object.ref:setDirection(object.direction)
  if  object.ref:getPositionX() < 0  then
    object.ref:stop()
    object.direction = MovingDirection.stayStill
    object.ref:resetVelocity()
    object.ref:respawn()
  end
end
State18.Npc.Show =  State:new()  
State18.Npc.Show.execute = function(npc) 

  if (npc.ref:getDistance().x >= ScreenWidth * 0.45  ) then 
    npc.ref:stop()
    return
  end
  npc.ref:setBackward()
end
return State18
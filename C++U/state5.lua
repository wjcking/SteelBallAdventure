local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local State = require("state")
local CommonStates = require("commonstates")
local State5 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, }

--玩家钢蛋
State5.Player.Routine = State:new()
State5.Player.Routine.execute = function(player)
  getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical)
  player.ref:checkSpike()
  PlayerStates.Routine.execute(player)
 
  local bound = player.ref:getBoundSelf().tilePosition
  --存档
  if (  Checkpoint.id == 0 and bound.x >= 14 and bound.y <= 25) then
    --第一阶段
    Checkpoint.setPtc(Vec2(14,24))  
  elseif (Checkpoint.id == 1 and bound.x < 4 and bound.y <= 13) then
    --第二阶段 
    Checkpoint.setPtc(Vec2(3,13))
    Role[RoleIndex.flatBoard].hintIndex = 4
      Role[RoleIndex.flatBoard].resetBoard(4)
  end
end
State5.Player.Routine.onMessage = function(player, telegram)

  if (telegram.message == MessageType.trigger) then 
    if (telegram.sender.name == "block") then

    end

    if (telegram.sender.name == "start") then

    end
  end 
end
--第一阶段和第二阶段都有
State5.Triggers.Spike = State:new()
State5.Triggers.Spike.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  --开头左侧倒刺
  if (bound.y == 0) then
     Ref.stage:promptAchieve(AchieveIndex.PlayerFall1,PopupInfo.achieve(AchieveIndex.PlayerFall1))
    Role[1].ref:gotoHell(Sound.spike)
    getStage():setGidRange(GidFlat.spikeRed,Vec2(8,34),Vec2(9,34))
    trigger.fsm:clearState()
  end
--  if ((bound.x == 5 or bound.x == 6) and bound.y==17) then
--    Role[1].ref:gotoHell(Sound.spike)
--    getStage():setGid(GidFlat.spikeRed,Vec2(5,17))
--    trigger.fsm:clearState()
--  end
end

State5.Triggers.Spike2 = State:new()
State5.Triggers.Spike2.execute = function(trigger)

  local bound = Role[1].ref:getBoundSelf().tilePosition

  --开始的方块砖头底下的倒刺
  if ((bound.x == 14 or bound.x == 15) and bound.y == 17) then

    Role[1].ref:gotoHell(Sound.spike)
    getStage():setGidRange(GidFlat.spikeRed,Vec2(14,17),Vec2(15,17))
    trigger.fsm:clearState()
  end
end
--三颗刺 第二阶段
State5.Triggers.Spikes = State:new()
State5.Triggers.Spikes.execute = function(object)

  local bound = Role[1].ref:getBoundSelf().tilePosition

  if (object.tag == RoleIndex.spikeLeft1) then
    if ( not object.isShaking and bound.x == 7 and bound.y >=22 and bound.y<=23  ) then
      object.ref:setAnimation(Animate.jumpShake)
      object.isShaking=true
    end

    if (Role[RoleIndex.buttonCenter].switchResult ~= nil and bound.x == 11 and bound.y == 21) then
      object.ref:stopAnimation()
      object.ref:setBackward()
      object.fsm:clearState()
    end
  end

  if (object.tag == RoleIndex.spikeLeft2) then
    if (Role[RoleIndex.buttonCenter].switchResult ~= nil and bound.x > 5 and  bound.y ==18) then
      local rebound = {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(-ScreenWidth/3.4,0), duration=0.5, durationBack=2, delay=0,delayBack=2, allowBack=true,} 
      object.ref:setAnimation(rebound)
      object.fsm:clearState()
    end
  end

--  if (object.tag == RoleIndex.spikeTop) then
--    if (Role[RoleIndex.buttonCenter].switchResult ~= nil and bound.x == 6 and bound.y == 23) then
--      local rebound = {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,-50), duration=0.5, durationBack=0.5, delay=0,delayBack=0, allowBack=true,} 
--      object.ref:setAnimation(rebound)
--      object.fsm:clearState()
--    end
--  end
end
--***********按钮事件************

local PushedKey = ""
State5.refresh = function()
  PushedKey = ""
end
State5.RObject.ButtonRGB = State:new()
State5.RObject.ButtonRGB.execute = function(object)
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end

      local switchResult = object.ref:switchFrame(Role[1].ref)
      --armeabi gcc 不能用父类FrameIndexes注册lua所以声明此if (switchResult.frameIndex > 1) then
      if (switchResult:getIndex() > 1 ) then
        PushedKey = PushedKey..tostring(object.tag)

        if (#PushedKey == 3) then
          if ("423" == tostring(PushedKey)) then
            Role[RoleIndex.flatBoard].hintIndex = 2
            Role[RoleIndex.btnBottomRight].ref.isVisible = true
            Role[RoleIndex.btnTopLeft].ref.isVisible = true
            Role[RoleIndex.btnTopRight].ref.isVisible = true
            Role[RoleIndex.btnBottomLeft].ref.isVisible = true
            Role[RoleIndex.btnBottomRight].fsm:changeState(State5.RObject.ButtonCorner)
            Role[RoleIndex.btnTopLeft].fsm:changeState(State5.RObject.ButtonCorner)
            Role[RoleIndex.btnTopRight].fsm:changeState(State5.RObject.ButtonCorner)
            Role[RoleIndex.btnBottomLeft].fsm:changeState(State5.RObject.ButtonCorner)
            Role[RoleIndex.block1].ref:allowPush(CollisionDirection.intersected)
            Role[RoleIndex.block2].ref:allowPush(CollisionDirection.intersected)
            Role[RoleIndex.block1].fsm:changeState(RObjectStates.gotPushed)
            Role[RoleIndex.block2].fsm:changeState(RObjectStates.gotPushed)

        else
          getStage():setGidRange(GidFlat.spikeBackRed,Vec2(1,26),Vec2(16,33))
          end

          --重置否则内存会累加
          PushedKey = ""
        end
        object.fsm:clearState()
      end 
    end) 
end
--四个角落的按钮
State5.RObject.ButtonCorner = State:new()
State5.RObject.ButtonCorner.execute = function(object)
  object.getOpponents(object,function(range,opponent)

      if (range < RangeType.rangeThreatened) then 
        return
      end

      object.switchResult = object.ref:switchFrame(opponent.ref)
      --armeabi gcc 不能用父类FrameIndexes注册lua所以声明此if (switchResult.frameIndex > 1) then
      if (object.switchResult:getIndex() > 1 ) then

        if (object.tag == RoleIndex.btnBottomLeft) then
          getStage():setGidRange(0,Vec2(8,34),Vec2(9,34))
          Resh.playSound(Sound.enigma)
          object.fsm:clearState()
        elseif(object.tag == RoleIndex.btnTopLeft) then
          getStage():setGidRange(GidFlat.spikeBackRed,Vec2(3,26),Vec2(3,28))
          getStage():setGidRange(GidFlat.spikeBackRed,Vec2(2,28),Vec2(3,28))
        elseif(object.tag == RoleIndex.btnTopRight) then 
          getStage():setGidRange(GidFlat.spikeBackRed,Vec2(14,28),Vec2(16,28))
          getStage():setGid(GidFlat.spikeBackRed,Vec2(14,27))
        elseif(object.tag == RoleIndex.btnBottomRight) then
          getStage():setGidRange(GidFlat.spikeBackRed,Vec2(14,31),Vec2(16,31))
          getStage():setGidRange(GidFlat.spikeBackRed,Vec2(14,32),Vec2(14,33))
        end

      end 
    end) 
  --开门，进入第二
  if (Role[RoleIndex.btnTopLeft].switchResult ~= nil and
    Role[RoleIndex.btnTopRight].switchResult ~= nil and
    Role[RoleIndex.btnBottomRight].switchResult ~= nil and
    Role[RoleIndex.btnTopLeft].switchResult:getIndex() > 1 and
    Role[RoleIndex.btnTopRight].switchResult:getIndex() > 1 and
    Role[RoleIndex.btnBottomRight].switchResult:getIndex() > 1) then
    Role[RoleIndex.flatBoard].hintIndex = 3 
    Role[RoleIndex.flatBoard].resetBoard(3)
    object.fsm:clearState()
    
    Role[RoleIndex.btnBottomLeft].isDisposed = true
    Role[RoleIndex.btnTopLeft].isDisposed = true
    Role[RoleIndex.btnTopRight].isDisposed = true
    Role[RoleIndex.btnBottomRight].isDisposed = true

    Role[RoleIndex.btnRed].isDisposed = true
    Role[RoleIndex.btnGreen].isDisposed = true
    Role[RoleIndex.btnBlue].isDisposed = true

    Role[RoleIndex.block1].isDisposed = true
    Role[RoleIndex.block2].isDisposed = true

    getStage():setGidRange(0,Vec2(1,26),Vec2(16,33))
    getStage():setGidRange(0,Vec2(14,25),Vec2(15,25))
  end
end
State5.RObject.WallBlock = State:new()
State5.RObject.WallBlock.execute = function(object)
  if (object.tag == RoleIndex.wallBlock1) then
    if (object.ref:getBoundSelf().tilePosition.x == 15 ) then

      Role[RoleIndex.spikeTop].isDisposed = true
    end
  else
    if (Role[RoleIndex.wallBlock2].ref:getBoundSelf().tilePosition.x > 16 and
      Role[RoleIndex.wallBlock3].ref:getBoundSelf().tilePosition.x > 16) then
      Resh.playSound(Sound.enigma)
      Triggers[2].fsm:clearState()
      object.isDisposed = true
    end

  end
end
State5.RObject.WallBlock.onMessage = function(object, telegram)
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end
end
--双星
State5.RObject.DoubleSpikes = State:new()
State5.RObject.DoubleSpikes.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if ( object.rebound == nil and bound.x >= 14 and bound.y == 16  ) then
    local rebound = {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(ScreenWidth - 320,0), duration=0.5, durationBack=2, delay=0,delayBack=2, allowBack=true,} 
    object.rebound = 1
    object.ref:setAnimation(rebound)
  end
  --Luah.output("[State5.RObject.DoubleSpikes]"..object.rebound)
  --踩按钮后
  if (Role[RoleIndex.buttonCenter].switchResult ~= nil and  bound.x >= 3 and bound.y == 16) then
    local rebound = {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(ScreenWidth - 250,0), duration=1, durationBack=1, delay=0,delayBack=0, allowBack=true,} 
    object.fsm:clearState()
    object.ref:setAnimation(rebound)
  end
end
--按钮第二阶段
State5.RObject.ButtonCenter = State:new()
State5.RObject.ButtonCenter.execute = function(object)
  local switchResult = object.ref:switchFrame(Role[1].ref)

  if (switchResult:getIndex() > 1 ) then
    object.switchResult = switchResult
    getStage():setGidRange(0,Vec2(2,14),Vec2(3,14))
    getStage():setGid(0,Vec2(5,17))
  --  getStage():setGidRange(GidFlat.blockWall,Vec2(16,21),Vec2(17,23))
    getStage():setGidRange(GidFlat.pad,Vec2(2,15),Vec2(3,16))
    --  getStage():setGidRange(GidFlat.pad,Vec2(4,23),Vec2(13,23))
    Role[RoleIndex.wallBlock1].ref:allowPush(CollisionDirection.intersected)
    Role[RoleIndex.wallBlock1].fsm:changeState(State5.RObject.WallBlock)
    object.fsm:clearState()
  end
end
--*****
--**第三阶段
--***

State5.RObject.FinalSpike = State:new()
State5.RObject.FinalSpike.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if (bound.x  > 9 and bound.y < 14) then
    local rebound = {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(-ScreenWidth /2,0), duration=3, durationBack=3, delay=0,delayBack=0, allowBack=true,} 
    object.ref:setAnimation(rebound)
     Resh.playSound(Sound.enigma)
    Role[1].fsm:changeState(State5.Player.ThroughWalls)
    getStage():setGidRange(0,Vec2(2,10),Vec2(3,10))
    object.fsm:clearState()
  end
end


State5.Player.ThroughWalls = State:new()
State5.Player.ThroughWalls.execute = function(player)
  local bound = player.ref:getBoundSelf().tilePosition
  if (bound.x < 4 and bound.y == 10) then
    player.ref:setZOrder(0) 
  end
  if (bound.x < 4 and bound.y == 8) then
    getStage():setObjectGid(0,Vec2(2,5),Vec2(3,10))
    getStage():setObjectGid(0,Vec2(4,5),Vec2(15,6))
    getStage():setObjectGid(0,Vec2(14,7),Vec2(15,10))
    player.fsm:clearState()
  end 
end
return State5
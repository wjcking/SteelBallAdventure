local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local State = require("state")

local State2 = { Player={}, Npc={},RObject={}, ProjectTile={},Trigger = {} , Dialog={},}

State2.Player.Routine = State:new()
State2.Player.Routine.execute = function(player)
  getStage():setCameraCenter(player.ref, CameraView.horizontal,false)
  --Joystick.ref:setVisible("drop", player.ref:isCarrying())
  PlayerStates.Routine.execute(player)

  local bound = player.ref:getBoundSelf().tilePosition
  if (Checkpoint.id <= 0 and bound.x >= 13 ) then
    Checkpoint.setPtc(Vec2(13,17))
  end
  if ( bound.x >= 48 ) then
    player.ref:freeze()
    player.fsm:clearGlobal()
    player.fsm:changeState(State2.Player.ToTheEnd)
  end
end
State2.Player.ToTheEnd = State:new()
State2.Player.ToTheEnd.execute = function(player)
  --- getStage():setCameraCenter(player.ref, false,false)
  --Joystick.ref:setVisible("drop", player.ref:isCarrying())
  PlayerStates.Routine.execute(player)

  local isDestiny = getStage():moveCameraX(3,400)

  if (isDestiny) then
    player.fsm:clearState()
    Segment[1].fsm:changeState(State2.Dialog.Zzi)
  end
end

State2.Trigger.Spike = State:new() 
State2.Trigger.Spike.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf()

  --开头左侧倒刺
  if (bound.tilePosition.x < 3 and bound.tilePosition.y == 17) then
    Role[1].ref:gotoHell(Sound.spike)
    getStage():setGidRange(Gid.spikeBottom,Vec2(0,17),Vec2(2,17))
    trigger.fsm:clearState()
  end
  --开始的方块砖头底下的倒刺
  if (bound.tilePosition.x == 3 and bound.tilePosition.y == 16) then
    Role[1].ref:gotoHell(Sound.spike)
    getStage():setGid(Gid.spikeTop,Vec2(3,16))
    trigger.fsm:clearState()
  end
end

State2.Trigger.CollapseStart = State:new() 
State2.Trigger.CollapseStart.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf()
  --  Luah.output("X:"..bound.tilePosition.x.." Y:"..bound.tilePosition.y)
  if (bound.tilePosition.x > 8 ) then
    getStage():collapse({ rangeStart = Vec2(9,18), rangeEnd = Vec2(11,19),step=Vec2(2,2), direction=MovingDirection.toBottom,distance=Vec2(90,90), })
    trigger.fsm:clearState()
  end
end

State2.Npc.Agnail = State:new() 
State2.Npc.Agnail.execute = function(npc)

  local bound = Role[1].ref:getBoundSelf()

  if (bound.tilePosition.x == 4 and bound.tilePosition.y == 14) then
    npc.ref:setAnimation(Animate.reboundAgnail)
    npc.fsm:clearState()
  end
end
--小苹果动作
State2.RObject.Fall = State:new() 
State2.RObject.Fall.execute = function(object)

  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (object.tag == RoleIndex.apple1) then
    if bound.x == object.pos.x-1 then
      object.ref.isHarmful = false
      object.ref:setAnimation({ type = Anistyle.rebound,duration=0.1,durationBack=0.1,offset= Vec2(0,-130)})
      object.fsm:clearState()
    end
  elseif object.tag == RoleIndex.appleLast then
    if bound.x == object.pos.x-1 then
      object.ref.isHarmful = false
      object.ref:setAnimation({ type = Anistyle.rebound,duration=0.1,durationBack=0.1,offset= Vec2(0,-120)})
      object.fsm:clearState()
    end
  end
end
State2.RObject.Fall.onMessage = function(object, telegram)

end
State2.RObject.FallAndSneakAttack =  State:new() 
State2.RObject.FallAndSneakAttack.execute = function(object)


  if (Role[1].ref:getBoundSelf().tilePosition.x >= 33) then
    object.ref.moveStep = Vec2(2.5,2.5)
    object.ref:showHints(RObjectText.appleSneak)
    object.ref:setForward()
  end

  --超过则销毁
  if (object.ref:getDistance().x >= ScreenWidth) then
    object.ref:closeDialog()
    object.isDisposed = true
  end


  if (Role[1].ref:getBoundSelf().tilePosition.x == object.pos.x-1) then
    --声音fall播放一次
    if (not object.isFallen) then
      Resh.playSound(Sound.fall)
      object.isFallen = true
    end
    object.ref:setGravityOn()
  end


end
State2.RObject.FruitTrace =  State:new() 
State2.RObject.FruitTrace.execute = function(object)

  local isDestiny = object.ref:locate( Role[1].ref:getPosition(), true)

  if (isDestiny ) then
    
    object.isDisposed = true
     if Segment[1].fsm:isInState(State2.Dialog.Zzi) then
       
    Ref.stage:promptAchieve(AchieveIndex.LittleApple1,PopupInfo.achieve(AchieveIndex.LittleApple1))
     end
  end

end
State2.RObject.QuestionGreen =  State:new() 
State2.RObject.QuestionGreen.execute = function(object)

  object.getPlayer(object,function(range,player)
      --注意这里，距离判断，否则不触发
      if (range < RangeType.rangeSensed) then 
        return
      end 
      local isCollided = object.ref:checkObjectCollision(player.ref,false)
      if (isCollided) then
        Role[RoleIndex.fruitTrace].fsm:changeState(State2.RObject.FruitTrace)
     
        object.isDisposed = true
      else

      end
    end)
  --超过则销毁
  if (object.ref:getDistance().y >= 200) then
    object.isDisposed = true
  end
end
State2.RObject.SneakTrace =  State:new() 
State2.RObject.SneakTrace.execute = function(object)

  if (object.ref.isHarmful) then
    local isDestiny = object.ref:locateTile(Vec2(39,14), true)
    --超过则销毁
    if (isDestiny ) then
      object.isDisposed = true
    end
    return
  end

  local bound = Role[1].ref:getBoundSelf().tilePosition
  --云彩下面袭击
  if (bound.x  >= 38 and  bound.y == 14) then
    object.ref.isHarmful= false
  end
end
--和狗比张，猪鼻脸对话
State2.Dialog.Zzi= State:new() 
State2.Dialog.Zzi.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end 
State2.Dialog.Zzi.execute = function(segment)

  DialogueSystem.update()
  if (DialogueSystem.isDone()) then
    segment.fsm:changeState(State2.Passed)
  end

end
State2.RObject.Knocked= State:new() 
State2.RObject.Knocked.execute= function(object)
  object.ref:checkKnocks(Role[1].ref)
end 
--敲
State2.RObject.Knocked.onMessage = function(object, telegram)
  if (telegram.message == MessageType.knocked) then
    object.ref:gotKnocked(telegram.sender.ref)
  end
end
State2.RObject.Cloud= State:new() 
State2.RObject.Cloud.onMessage = function(object, telegram)
  if (telegram.message == MessageType.hit) then
    object.ref:setFrame("cloud12.png")
    Ref.stage:promptAchieve(AchieveIndex.Cloud,PopupInfo.achieve(AchieveIndex.Cloud))
  end
end
State2.Passed= State:new() 
State2.Passed.enter = function(segment)
  Role[1].ref:closeDialog()
  Role[RoleIndex.zzi].ref:closeDialog()
  Role[RoleIndex.zzi].ref:stopAnimation()
end 
State2.Passed.execute = function(segment)
  --往后退
  Role[RoleIndex.zzi].ref:setForward()
  if (Delay.isTimeUp(1, 3)) then
    Ref.stage:prompt(PopupInfo.passed())
    segment.fsm:clearState()
  end
end
--后补

State2.RObject.DialogBoard = State:new()
State2.RObject.DialogBoard.execute = function(object)

  if  not object.ref:inAir(false) and  object.ref.allowGravity then
    object.ref.allowGravity= false
    object.ref.isHarmful =false 
  end

  if  object.isTouched == nil and object.ref:getTouchTimes() > 0 and object.touchedText ~= nil  then 
    object.isTouched = true
    Resh.playSound(Sound.fall)
    object.ref:registerText(object.touchedText) 
    object.ref.allowGravity = true
    object.ref.isHarmful =true
  end

  if (object.replacedText ~= nil and Role[1].ref:isDead()) then
    object.ref:registerText(object.replacedText)
    object.fsm:clearState()
  end
end

return State2
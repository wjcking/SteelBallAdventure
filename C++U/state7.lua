local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State7 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}
local WrongNumber = 0
State7.Player.Routine = State:new()
State7.Player.Routine.execute = function(player)

  PlayerStates.Routine.execute(player)
  player.ref:checkSpike()

end
State7.Player.Routine.onMessage = function(player, telegram)
  --成就zzi舔b
  if telegram.message == MessageType.hit and telegram.info.tag == RoleIndex.zzi and not Role[RoleIndex.zzi].ref.allowGravity then
    Ref.stage:promptAchieve(AchieveIndex.ZziCollided,PopupInfo.achieve(AchieveIndex.ZziCollided))
  end
end
State7.Triggers.Trap = State:new()
State7.Triggers.Trap.execute = function(trigger)

  if  Segment[1].question > 2 then
    trigger.fsm:clearState()
    return
  end
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if bound.x == 7 and bound.y==12 then
    getStage():setGid(Gid.spikeBottom,Vec2(7,12))
    trigger.fsm:clearState()
  end
end

State7.RObject.BlockChoice = State:new()
State7.RObject.BlockChoice.execute = function(object)
  object.ref:checkKnocks(Role[1].ref)
end
State7.RObject.BlockChoice.onMessage = function(object, telegram)
  if (telegram.message == MessageType.knocked) then
    object.ref:gotKnocked(telegram.sender.ref) 
    MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,Role[RoleIndex.zzi],MessageType.knocked)
  end
end


State7.Dialogue.Dialog = State:new()
State7.Dialogue.Dialog.enter = function(segment)
  Role[1].ref:freeze()
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end
State7.Dialogue.Dialog.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then 
    Role[1].ref:unfreeze() 
    Role[RoleIndex.zzi].fsm:changeState(State7.Npc.ZziQuestion)
    segment.fsm:clearState()
    Checkpoint.id = Checkpoint.id + 1
  end
end
State7.Dialogue.Dialog.exit = function(segment)
  DialogueSystem.close()
end

local function zziLaugh(npc)
  --嘲笑
  if (Role[1].ref:isDead()) then  
    npc.ref:closeDialog()
    npc.ref:stop()
    npc.ref:stopAnimation()
    npc.ref:setAnimation( { type = Anistyle.jump, height=50,duration=1.5,limitedTimes=3})
    npc.fsm:clearState()
    return true
  end

  return false
end
State7.Npc.ZziQuestion = State:new()
State7.Npc.ZziQuestion.enter = function(npc)
  Resh.playSound(Sound.question)
end
State7.Npc.ZziQuestion.execute = function(npc)

  if (zziLaugh(npc)) then
    return
  end
  if (Role[RoleIndex.brick1].isDisposed and
    Role[RoleIndex.brick2].isDisposed and
    Role[RoleIndex.brick3].isDisposed and
    not npc.ref.allowGravity) then
    npc.ref.allowGravity = true
  end
  --下落了并且屎已经打开
  if (npc.allowFeed and npc.ref.allowGravity) then
    npc.ref:closeDialog()
    npc.ref:patrollingX(true)

    npc.getOpponents(npc,function(range,opponent)
        if (opponent.frame == "poo.png" or opponent.frame == "drink2.png") then
          if ( range < RangeType.rangeThreatened) then 
            return
          end
          local isCollided = npc.ref:checkObjectCollision(opponent.ref,false)
          if (isCollided) then
            if (opponent.frame == "poo.png") then
              opponent.isDisposed = true
            end
            if (opponent.frame == "drink2.png") then
              npc.ref:setFrame("zzi2.png")
              npc.ref:gotoHell(Sound.manDied)
              opponent.isDisposed = true
              --  if (Delay.isTimeUp(DelayIndex.passed, 2)) then
              Ref.stage:prompt(PopupInfo.passed())
              npc.fsm:clearState()
              --  end
            end
          end
        end
      end)
  else
    npc.ref:showHints(Question[Segment[1].question])
  end
end
State7.Npc.ZziQuestion.exit = function(npc)

end
State7.Npc.ZziQuestion.onMessage = function(npc, telegram)
  --如果没有char，可能多次接受消息
  if (nil == telegram.sender.char) then
    return
  end

  if (telegram.sender.char.text == Question[Segment[1].question].key) then
    Resh.playSound(Sound.coin)
    npc.isCorrect = true
  else
    Resh.playSound(Sound.incorrect)
    npc.isCorrect = false
  end
  npc.fsm:changeState(State7.Npc.ZziResult)
  -- 问题别越界
  if (Segment[1].question <  #Question) then
    Segment[1].question = Segment[1].question + 1
  end
  Delay.reset(DelayIndex.common)
end


State7.Npc.ZziResult = State:new()
State7.Npc.ZziResult.enter = function(npc)
  npc.ref:closeDialog()
  npc.isExecuted = false
  if (npc.giftNumber == nil) then
    npc.giftNumber = 0
  end
end
State7.Npc.ZziResult.execute = function(npc)
  if (zziLaugh(npc)) then
    return
  end
  if (npc.isCorrect) then
    npc.ref:showHints(Question[Segment[1].question-1].correct)
  else
    npc.ref:showHints(Question[Segment[1].question-1].incorrect)
  end
  --回答错误
  if (not npc.isCorrect and not npc.isExecuted ) then
    WrongNumber = WrongNumber + 1
    if   WrongNumber >= 3 then
      Ref.stage:promptAchieve(AchieveIndex.WrongAnswers,PopupInfo.achieve(AchieveIndex.WrongAnswers))
    end
    Role[RoleIndex.char1].isDisposed=true
    Role[RoleIndex.char2].isDisposed=true
    Role[RoleIndex.char3].isDisposed=true
    Role[RoleIndex.char4].isDisposed=true
    local punishStyle =math.random(1,3)
    if  punishStyle == 1  then
      getStage():setGidRange(Gid.brick,Vec2(1,9),Vec2(16,9))
      local missile = {type=RoleType.npc,frame="missile_blue.png",step=Vec2(1,1),pos=Vec2(19,10), offset=Vec2(0,-15), onCheckTile=false,isSolid=false, allowGravity=false, direction=MovingDirection.toLeft, zorder=-1,}
      local missile2 = {type=RoleType.npc,frame="missile_red.png",step=Vec2(1,1),pos=Vec2(19,12), offset=Vec2(0,15), onCheckTile=false,isSolid=false, allowGravity=false, direction=MovingDirection.toLeft, zorder=-1,}
      local missile3 = {type=RoleType.npc,frame="missile_blue.png",step=Vec2(1,1),pos=Vec2(0,10), offset=Vec2(0,-15), onCheckTile=false,isSolid=false, allowGravity=false, direction=MovingDirection.toRight, zorder=-1,}
      local missile4 = {type=RoleType.npc,frame="missile_red.png",step=Vec2(1,1),pos=Vec2(0,12), offset=Vec2(0,15), onCheckTile=false,isSolid=false, allowGravity=false, direction=MovingDirection.toRight, zorder=-1,}
      missile.fsm = Fsm:new(missile,nil,CommonStates.Move) 
      missile2.fsm = Fsm:new(missile2,nil,CommonStates.Move)
      missile3.fsm = Fsm:new(missile3,nil,CommonStates.Move) 
      missile4.fsm = Fsm:new(missile4,nil,CommonStates.Move)
      createRole(missile)
      createRole(missile2) 
      createRole(missile3)
      createRole(missile4) 
      Resh.playSound(Sound.missile)
    elseif (punishStyle == 2) then
      getStage():removeTileRange( Vec2(1,13),Vec2(16,14))
    elseif (punishStyle == 3) then
      getStage():setGidRange(Gid.spikeBottom,Vec2(1,12),Vec2(16,12))
    end
    --答对了
  elseif (npc.isCorrect and not npc.isExecuted ) then 
    if (Segment[1].question == 4) then
      local gift = {type=RoleType.robject,frame="gift_yellow.png", replacedFrame="poo.png", pos=Vec2(6 + npc.giftNumber,7), allowGravity=true, isSolid=false,}
      gift.fsm = Fsm:new(gift,nil,State7.RObject.Gift)
      Resh.playSound(Sound.fall)
      createRole(gift)
    elseif (Segment[1].question == 5) then
      local gift = {type=RoleType.robject,frame="gift_orange.png", replacedFrame="drink2.png", pos=Vec2(9 + npc.giftNumber,7), allowGravity=true, isSolid=false,}
      gift.fsm = Fsm:new(gift,nil,State7.RObject.Gift)
      Resh.playSound(Sound.fall)
      createRole(gift)
    elseif (Segment[1].question == 6) then
      local gift = {type=RoleType.robject,frame="gift_cyan.png",  replacedFrame="helmet.png", pos=Vec2(12 + npc.giftNumber,7), allowGravity=true, isSolid=false,}
      gift.fsm = Fsm:new(gift,nil,State7.RObject.Gift)
      Resh.playSound(Sound.fall)
      createRole(gift)
    elseif Segment[1].question == 3 then
      local missile  = {type=RoleType.npc,frame="missile_red.png",step=Vec2(4.5,4.5),pos=Vec2(0,11), offset=Vec2(0,-10), onCheckTile=false,isSolid=false, allowGravity=false, direction=MovingDirection.toRight, zorder=-1,}
      missile.fsm = Fsm:new(missile,nil,CommonStates.Move) 
      Resh.playSound(Sound.missile)
      createRole(missile)
    end
    npc.giftNumber = npc.giftNumber + 1
  end 
  npc.isExecuted = true
  if (npc.isCorrect and Delay.isTimeUp(DelayIndex.common, 2)) then
    npc.fsm:changeState(State7.Npc.ZziQuestion)
  end
end


State7.RObject.Gift = State:new()
State7.RObject.Gift.execute = function(object)

  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local playerBound = player.ref:getBoundSelf().tilePosition
      local objectBound = object.ref:getBoundSelf().tilePosition
      local isShow = false
      if ( object.frame ~= object.replacedFrame and 
        playerBound.y == objectBound.y and
        (playerBound.x == objectBound.x + 1 or playerBound.x == objectBound.x -1)) then
        -- 如果没打开并且在礼品旁边
        isShow = true
      else
        isShow = false
      end
      Joystick.ref:setVisible(CommandPad.open, isShow)
      --打开礼品
      if (isShow and Joystick.ref:getTouchedPad(2)) then 
        object.frame = object.replacedFrame
        object.ref:setFrame(object.replacedFrame)
        --如果不是有害的，发送消息
        if ( object.replacedFrame == "poo.png") then
          Role[RoleIndex.zzi].allowFeed = true
        end
        if (object.replacedFrame == "drink2.png" or object.replacedFrame == "poo.png") then
          object.ref.isHarmful = true 
        end
        object.allowNotify = true
      end     
    end)
end
State7.RObject.Gift.onMessage = function(object,telegram)
  if (object.frame == "drink2.png") then
   -- Role[1].fsm:clearGlobal()
    Resh.playSound(Sound.manDied)
    object.isDisposed = true
    return
  end
  if (telegram.message ~= MessageType.collider) then
    return
  end

  --如果礼品已经打开
  if (object.frame == object.replacedFrame) then
    if (object.frame == "helmet.png") then

      Ref.stage:prompt(PopupInfo.createObject(Folder.texture.."helmet.png",RObjectText.helmet))
      local knockCorrupt =   {type = KnockAction.corrupt,direction=CollisionDirection.atBottom, sound=Sound.broken,limitedTimes=1,} 

      Role[RoleIndex.brick1].ref:registerKnocks(knockCorrupt)
      Role[RoleIndex.brick2].ref:registerKnocks(knockCorrupt)
      Role[RoleIndex.brick3].ref:registerKnocks(knockCorrupt)

      Role[RoleIndex.brick1].fsm:changeState(RObjectStates.Knocked)
      Role[RoleIndex.brick2].fsm:changeState(RObjectStates.Knocked)
      Role[RoleIndex.brick3].fsm:changeState(RObjectStates.Knocked)
    end

    object.isDisposed = true
  end
end


return State7
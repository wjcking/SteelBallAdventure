local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State12 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}
local tileRemoval = 4
local bastardTag = 0
local grumbleID = 1
local correctCount = 0
local answerCount = 0
State12.initialize = function()
  tileRemoval = 4
  bastardTag = RoleIndex.kid
  correctCount = 0
  answerCount = 0
end
local function laugh(npc)
  --嘲笑
  if (Role[1].ref:isDead() and npc.ref:isAlive()) then  
    npc.ref:closeDialog()
    npc.ref:stop()
    npc.ref:setAnimation( { type = Anistyle.jump, height=50,duration=1.5,limitedTimes=3})
    npc.fsm:clearState()
    return true
  end

  return false
end


--对话
State12.Dialogue.Dialog = State:new()
State12.Dialogue.Dialog.enter = function(segment)
  Role[1].ref:freeze()
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end
State12.Dialogue.Dialog.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then 
    Role[1].ref:unfreeze() 
    segment.fsm:clearState()
    Role[RoleIndex.kid].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.man].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.woman].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.lady].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.cowboy].fsm:changeState(State12.Npc.Bastard)

    Checkpoint.id = Checkpoint.id + 1
    Role[RoleIndex.wangba].fsm:changeState(State12.Npc.DickQuestion)
  end
end
State12.Dialogue.Dialog.exit = function(segment)
  DialogueSystem.close()
end


State12.Player.Routine = State:new()
State12.Player.Routine.execute = function(player)
  getStage():setCameraSlide(player.ref:getPosition(),CameraView.horizontal)
  PlayerStates.Routine.execute(player)
  Role[1].ref:checkSpike()

--  if (player.allowFire ~= nil) then
--    Joystick.ref:setVisible(CommandPad.fire,player.ref:getBoundSelf().tilePosition.x <19)
--  end
--    player.isReached = true
--  end
end
State12.Player.Routine.onMessage = function(player, telegram)
  if (telegram.message == MessageType.gotShot) then
    Role[1].ref:gotHurt(1,Sound.hurt)
  end

end
--混蛋们
State12.Npc.Bastard = State:new()
State12.Npc.Bastard.execute = function(npc)

  if (npc.isDisposed ) then
    return
  end 
  npc.ref:checkSpike()  
  --遍历每一个混蛋，
  if (Delay.isTimeUp(DelayIndex.spawn, 1)) then
    if (bastardTag <= RoleIndex.cowboy) then
      bastardTag = bastardTag + 1
    else
      bastardTag = RoleIndex.kid
    end

    grumbleID = math.random(1,5)
    Delay.reset(DelayIndex.spawn)
  end
  --嘴里嘟囔
  if (npc.tag == bastardTag and npc.ref:isAlive()) then
    npc.ref:showHints(Hints.grumble[grumbleID])
  else
    npc.ref:closeDialog()
  end
end

--敲击答案
State12.RObject.BlockChoice = State:new()
State12.RObject.BlockChoice.execute = function(object)
  object.ref:checkKnocks(Role[1].ref)
end
State12.RObject.BlockChoice.onMessage = function(object, telegram)
  if (telegram.message == MessageType.knocked) then
    object.ref:gotKnocked(telegram.sender.ref) 
    MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,Role[RoleIndex.wangba],MessageType.knocked)
  end
end


--迪克问
State12.Npc.DickQuestion = State:new()
State12.Npc.DickQuestion.enter = function(npc)
  if  not Role[RoleIndex.question].isDisposed then
    Resh.playSound(Sound.question)
  end
end
State12.Npc.DickQuestion.execute = function(npc)
  --如果dick死亡胜利
  if (npc.ref:isDead()) then
    Ref.stage:promptAchieve(AchieveIndex.SneakyShoot,PopupInfo.achieve(AchieveIndex.SneakyShoot))
    Ref.stage:prompt(PopupInfo.passed())
    npc.fsm:clearState()
  end
--主角死亡，嘲笑
  if (laugh(npc)) then
    return
  end
  --全部答对
  if (correctCount == #Question) then
    Role[RoleIndex.block1].isDisposed = true
    Role[RoleIndex.block2].isDisposed = true
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    npc.ref:showHints(Hints.correctForShooting)
     Ref.stage:promptAchieve(AchieveIndex.GotoHell,PopupInfo.achieve(AchieveIndex.GotoHell))
    return
  elseif (answerCount >= #Question) then
    Role[RoleIndex.block1].isDisposed = true
    Role[RoleIndex.block2].isDisposed = true
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    npc.ref:showHints(Hints.finishForShooting)
     Ref.stage:promptAchieve(AchieveIndex.GotoHell,PopupInfo.achieve(AchieveIndex.GotoHell))
    return
  elseif ( Role[RoleIndex.question].isDisposed ) then
    npc.ref:closeDialog() 
  else
    npc.ref:showHints(Question[Segment[1].question])
  end
  --答完题发射子弹
  if (npc.allowShoot and npc.ref:getBodyStatus() == BodyStatus.healthy) then
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    npc.allowShoot = false
  end

end

State12.Npc.DickQuestion.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.gotShot) then
    npc.ref:gotHurt(1,Sound.wangbaHurt)
    if (Role[1].ref:getBoundSelf().tilePosition.x < 15) then
      npc.allowShoot = true
    end
  end
  --如果没有char，可能多次接受消息
  if (nil == telegram.sender.char) then
    return
  end

  --问题是否问完

  if (telegram.sender.char.text == Question[Segment[1].question].key) then
    Resh.playSound(Sound.coin)
    npc.isCorrect = true
  else
    Resh.playSound(Sound.incorrect)
    npc.isCorrect = false 
  end 
  -- 问题别越界
  if (Segment[1].question <  #Question) then
    Segment[1].question = Segment[1].question + 1
  end
  --回答问题
  npc.fsm:changeState(State12.Npc.DickResult)

  Delay.reset(DelayIndex.common) 

end
--钢弹答
State12.Npc.DickResult = State:new()
State12.Npc.DickResult.enter = function(npc)
  npc.ref:closeDialog()
  npc.isExecuted = false
end
State12.Npc.DickResult.exit = function(npc)
  tileRemoval = tileRemoval + 2
end
State12.Npc.DickResult.execute = function(npc)
  if (laugh(npc)) then
    return
  end

  if (npc.isCorrect) then
    npc.ref:showHints(Question[Segment[1].question - 1].correct)
  else
    npc.ref:showHints(Question[Segment[1].question - 1].incorrect)
  end
  if (not npc.isExecuted) then
    -- --错了
    if (not npc.isCorrect) then
      getStage():removeTile(Vec2(tileRemoval,8))
      --   答对了
    elseif (npc.isCorrect  ) then 
      correctCount = correctCount + 1
    end
    --答题总数
    answerCount = answerCount + 1
  end
  npc.isExecuted = true
  if ( Delay.isTimeUp(DelayIndex.common, 2)) then
    npc.fsm:changeState(State12.Npc.DickQuestion)
  end
end

State12.RObject.DialogBoard = State:new()
State12.RObject.DialogBoard.execute = function(object)
  if not object.ref:inAir(false) then
    object.ref.allowGravity = false
    object.ref.isHarmful=false
  end
  if object.isTouched == nil and object.ref:getTouchTimes() > 0  then
    object.ref.allowGravity = true
    object.ref.isHarmful=true

    object.isTouched = true
  end
end
State12.RObject.DialogBoard.onMessage = function(object, telegram)
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end
end

State12.RObject.Question = State:new()
State12.RObject.Question.onMessage = function(object, telegram)
  local count = 0
  if (telegram.message == MessageType.collider) then
    Joystick.ref:setVisible(CommandPad.fire,true)
    getStage():removeTileRange(Vec2(17,5),Vec2(18,8))
    getStage():removeTileRange(Vec2(4,8),Vec2(12,12))
    for i = RoleIndex.kid,RoleIndex.cowboy,1 do
      if Role[i].ref:isDead() then
        count = count + 1
      end
    end

    if (count == 0) then
       Ref.stage:promptAchieve(AchieveIndex.Savior,PopupInfo.achieve(AchieveIndex.Savior))
    elseif count < 5 then
       Ref.stage:promptAchieve(AchieveIndex.Hero,PopupInfo.achieve(AchieveIndex.Hero))
    end
    Role[RoleIndex.kid].ref:closeDialog() 
    Role[RoleIndex.man].ref:closeDialog()   
    Role[RoleIndex.woman].ref:closeDialog() 
    Role[RoleIndex.lady] .ref:closeDialog() 
    Role[RoleIndex.cowboy].ref:closeDialog()

    Role[RoleIndex.kid].isDisposed = true
    Role[RoleIndex.man].isDisposed = true
    Role[RoleIndex.woman].isDisposed = true
    Role[RoleIndex.lady] .isDisposed = true
    Role[RoleIndex.cowboy].isDisposed = true
    
    Role[RoleIndex.block1].fsm:clearState()
    Role[RoleIndex.block2].fsm:clearState()
    Role[RoleIndex.block3].fsm:clearState()
    Role[RoleIndex.block4].fsm:clearState()
    
    Role[1].allowFire = true
    Resh.playSound(Sound.object)
    local  gunObject= {  count=-1,  sound=Folder.sound.."toygun.wav",rate=0.8, frame="bullet1.png",project={type=ProjectType.object, delay=3,allowGravity=false,step=Vec2(6,6)}}
    Role[1].ref:getWeapon():registerWeapon(gunObject)
    object.isDisposed = true
  end
end


State12.RObject.Spikes = State:new()
State12.RObject.Spikes.execute = function(object)
  if   Role[1].ref:getBoundSelf().tilePosition.x == object.pos.x then
    object.ref:setUpward()
  end
  if  object.ref:getDistance().y > ScreenHeight  then
    object.isDisposed=true
  end
end

State12.RObject.Spikes.onMessage = function(object, telegram)

end

return State12
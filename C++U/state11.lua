local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local CommonStates = require("commonstates")
local State = require("state")

local State11 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}
local IsSpringDispaced = false
local function displaceSpring()
  --一定要记录上一次位置
  if (not IsSpringDispaced) then
    Role[RoleIndex.spring1].originPosition = Role[RoleIndex.spring1].ref:getPosition()
    Role[RoleIndex.spring1].ref:displace(Role[RoleIndex.spring2].ref:getPosition())
    Role[RoleIndex.spring2].ref:displace( Role[RoleIndex.spring1].originPosition )
    IsSpringDispaced = true
  else
    Role[RoleIndex.spring2].originPosition = Role[RoleIndex.spring2].ref:getPosition()
    Role[RoleIndex.spring2].ref:displace(Role[RoleIndex.spring1].ref:getPosition())
    Role[RoleIndex.spring1].ref:displace(Role[RoleIndex.spring2].originPosition)
    IsSpringDispaced = false
  end

  Resh.playSound(Sound.trick)
end

State11.Player.Routine = State:new()
State11.Player.Routine.execute = function(player)
  getStage():setCameraCenter(player.ref, CameraView.horizontal,false)
  player.ref:checkSpike()
  PlayerStates.Routine.execute(player)

  local bound = player.ref:getBoundSelf().tilePosition

  if (Checkpoint.id == 0 and player.ref:isAlive() and bound.x > 35) then
    Checkpoint.setPtc(Vec2(36,14))  
  end
  if  bound.x == 107 and bound.y >= 16 and bound.y <= 19   then
    player.ref:setZOrder(ZOrder.cover)
    getStage():removeObjectRange(Vec2(105,11),Vec2(111,19))
  end
end
State11.Player.Routine.onMessage = function(player, telegram)
  if telegram.message == MessageType.hit and telegram.info.tag == RoleIndex.cloudDialog then
Ref.stage:promptAchieve(AchieveIndex.Cloud2,PopupInfo.achieve(AchieveIndex.Cloud2))
end
end
State11.Triggers.Trap= State:new()
State11.Triggers.Trap.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if ( trigger.isSpikeBottom == nil  and bound.x > 29 and bound.x < 32 and bound.y==12) then
    getStage():setGidRange(Gid.spikeBottom,Vec2(30,12),Vec2(31,12))
    trigger.isSpikeBottom = true
    Ref.stage:promptAchieve(AchieveIndex.PlayerFall3,PopupInfo.achieve(AchieveIndex.PlayerFall3))
    Role[1].ref:gotoHell(Sound.sting)
  end
  if ( trigger.isMissileLaunched == nil  and   bound.x == 32 and bound.y >= 13 and bound.y <=19) then
    trigger.isMissileLaunched = true
    local missile = {type=RoleType.npc,frame="missile_red_v.png",step=Vec2(4,4),pos=Vec2(32,20), offset=Vec2(0,0), onCheckTile=false,isSolid=false, allowGravity=false, isHarmful=false, direction=MovingDirection.toTop, zorder=-1,distance=Vec2(ScreenWidth,ScreenHeight*1.5),}
    Resh.playSound(Sound.missile)
    missile.fsm = Fsm:new(missile,nil,CommonStates.Move) 
    createRole(missile)
  end
  
   --第二个区域
  if trigger.isDispaced1 == nil and Role[RoleIndex.spring2].ref:getBoundSelf().tilePosition.x == 79  then
    trigger.isDispaced1 = true
    displaceSpring()
  end
  if trigger.isDispaced2 == nil and bound.x == 86 and bound.y >= 16 and bound.y <= 19 then
    trigger.isDispaced2 = true
    displaceSpring()
  end
  if trigger.isDoubleSpikeFallen == nil and bound.x == 90 and bound.y >= 16 and bound.y <= 19 then
    trigger.isDoubleSpikeFallen = true
    Role[RoleIndex.doubleSpikes].ref.allowGravity = true
  end
  if  bound.x == 105 and ( bound.y == 11 or bound.y==12) then
    getStage():setGidRange(Gid.spike,Vec2(105,11),Vec2(105,12))
    Resh.playSound(Sound.sneaky)
    Ref.stage:promptAchieve(AchieveIndex.DickSmile3,PopupInfo.achieve(AchieveIndex.DickSmile3))
    trigger.fsm:clearState()
  end


end

State11.RObject.CloudDialog= State:new()
State11.RObject.CloudDialog.execute = function(object)
  if (object.ref:getTouchTimes() > 0) then
    if (not object.ref.allowGravity) then
      Resh.playSound(Sound.fall)
    end
    object.ref.isHarmful = true
    object.ref.allowGravity = true
    object.ref:registerText(RObjectText.dialogReplaced)

    if (object.ref:getDistance().y >= ScreenHeight) then
      object.isDisposed = true
    end
  end
end

State11.RObject.Float= State:new()
State11.RObject.Float.execute = function(object)

  if (object.tag == RoleIndex.float4) then

    local bound = Role[1].ref:getBoundSelf().tilePosition
    if (bound.x == object.pos.x - 1) then
      object.fsm:changeState(CommonStates.Dodge)
    end
--  elseif (object.tag == RoleIndex.floatSpawn) then

--    local bound = Role[1].ref:getBoundSelf().tilePosition
--    if (bound.x >= 42 and bound.y < 13) then
--      object.ref:setFrame("float_hollow.png")
--      object.fsm:clearState()
--      Role[RoleIndex.hover1].isDisposed = true
--    elseif (bound.x >=42 and bound.y > 14 and bound.y < 19) then

--        object.fsm:changeState(State11.RObject.FloatMoveBySteps)
--    end
  end
end

State11.RObject.Float.onMessage = function(object, telegram)
  if (telegram.message == MessageType.collider) then
    
    if (object.tag == RoleIndex.float1) then
      object.fsm:changeState(CommonStates.Move)
    elseif (object.tag == RoleIndex.float2) then
      object.ref:setGravityOn()
    elseif (object.tag == RoleIndex.float3) then
      Role[RoleIndex.cloudDialog].fsm:changeState(State11.RObject.CloudDialog)
      object.fsm:clearState()
    elseif (object.tag == RoleIndex.float5) then
      object.fsm:changeState(CommonStates.Move)
      Role[RoleIndex.float4].fsm:changeState(State11.RObject.FloatForward)
    end
  end
end
--漂浮平台
State11.RObject.FloatForward = State:new()
State11.RObject.FloatForward.execute = function(object)

  local bound = Role[1].ref:getBoundSelf().tilePosition
  --到达顶部，左下旋梯移动
  if (bound.x > 24 and bound.y <= 12) then
    object.direction = MovingDirection.toRight
    object.distance = Vec2(ScreenWidth,ScreenHeight)
    object.ref.moveStep = Vec2(1,1)
    object.fsm:changeState(CommonStates.Move)
  end
end
--弹簧
State11.RObject.Spring = State:new()
State11.RObject.Spring.execute = function(object)
  --弹跳两次适应一下，就走
  if  Role[RoleIndex.spring1].bounceTimes  > 1 and Role[RoleIndex.spring2].bounceTimes  > 1 then
    object.ref:setForward()
  end
end

State11.RObject.Spring.onMessage = function(object, telegram)
  Resh.playSound(Sound.spring)
  object.bounceTimes = object.ref:bounce(Role[1].ref).times

  if (telegram.message == MessageType.collider) then

    if (object.tag == RoleIndex.spring1) then
      if (object.frame == "spring_red1.png") then
        object.frame = "spring_red4.png" 
      else
        object.frame = "spring_red1.png"
      end 
      --刷帧
      object.ref:setFrame(object.frame)
    else
      if (object.frame == "spring_gray1.png") then
        object.frame = "spring_gray4.png" 
      else
        object.frame = "spring_gray1.png"
      end 
      --刷帧
      object.ref:setFrame(object.frame)
    end

  end
end
--对话框
State11.RObject.DialogSpring= State:new()
State11.RObject.DialogSpring.execute = function(object) 
  if  Role[RoleIndex.spring1].bounceTimes  > 1 and Role[RoleIndex.spring2].bounceTimes  > 1 then
    object.ref:registerText(RObjectText.dialogSpringReplaced)
  end
  --放在最后面，
  if (Role[1].ref:isDead()) then
    object.ref:registerText(RObjectText.dialogSpringDead)
    object.fsm:clearState()
  end
  local bound = Role[1].ref:getBoundSelf().tilePosition
  --后面提示
  if (bound.x > 74) then
    object.fsm:changeState(State11.RObject.DialogEnd)
  end
end
--第二阶段
State11.RObject.DialogEnd= State:new()
State11.RObject.DialogEnd.enter = function(object)
  object.ref:spawn(Vec2(98,12),Vec2.ZERO)
  object.ref:registerText(RObjectText.dialogCrystal)
end
State11.RObject.DialogEnd.execute = function(object) 

  --放在最后面，
  if (Role[1].ref:isDead()) then
    object.ref:registerText(RObjectText.dialogSpringDead)
    object.fsm:clearState()
  end

end
--4个刺头
State11.Npc.Agnails = State:new()
State11.Npc.Agnails.execute = function(npc)
  if (Role[1].ref:isDead()) then
    npc.ref:setFrame("4agnails_red2.png")
    npc.fsm:clearState()
  end
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if (npc.tag == RoleIndex.agnail2) then
    if (bound.x == npc.pos.x - 1) then
      npc.ref:spawn(Vec2(npc.pos.x,12),Vec2.ZERO)
      Resh.playSound(Sound.charge)
      npc.fsm:clearState()
    end
  elseif (npc.tag == RoleIndex.agnail3) then
    if (bound.x == npc.pos.x + 4) then
      npc.fsm:changeState(CommonStates.Move)
    end
  elseif (npc.tag == RoleIndex.agnail4) then
    if bound.x == npc.pos.x - 1 and npc.isDisplacedSpring == nil then
       npc.isDisplacedSpring = true
      displaceSpring() 
   -- elseif bound.x >= 74 then
       --npc.fsm:changeState(CommonStates.Move)
    end
  end
end
State11.Npc.Agnails.onMessage = function(npc, telegram)

  if (telegram.message == MessageType.hit) then

    if (npc.frame == "4agnails_red1.png") then
      npc.frame = "4agnails_red2.png"
    else
      npc.frame = "4agnails_red1.png" 
    end  
    --刷帧
    npc.ref:setFrame(npc.frame)
  end
end
return State11
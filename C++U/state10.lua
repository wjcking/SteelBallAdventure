local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local CommonStates = require("commonstates")
local State = require("state")

local State10 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}

State10.Player.Routine = State:new()
State10.Player.Routine.execute = function(player)
  getStage():setCameraCenter(player.ref, CameraView.horizontal,false)
  player.ref:checkSpike()
  PlayerStates.Routine.execute(player)

  local bound = player.ref:getBoundSelf().tilePosition

  if (Checkpoint.id == 0 and player.ref:isAlive() and bound.x >= 23) then
    Checkpoint.setPtc(Vec2(23,17))
  end
  if (Checkpoint.id == 1 and player.ref:isAlive() and bound.x >= 54) then
    Checkpoint.setPtc(Vec2(54,17))
  end
end
State10.Player.Routine.onMessage = function(player, telegram)
  if telegram.message == MessageType.hit and telegram.info.tag == RoleIndex.reboundAgnail then
Ref.stage:promptAchieve(AchieveIndex.Agnail,PopupInfo.achieve(AchieveIndex.Agnail))
end
end
State10.Triggers.Trap = State:new()
State10.Triggers.Trap.execute = function(trigger)

  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (not  trigger.isCollapsed  and bound.x == 19 and bound.y <=15) then
    getStage():collapse({ rangeStart = Vec2(16,18), rangeEnd = Vec2(18,19),step=Vec2(1.6,1.6), direction=MovingDirection.toBottom,distance=Vec2(90,90), })
    trigger.isCollapsed = true
  end

  if (not trigger.isLaunched and bound.x == 20) then
    local missile = {type=RoleType.robject,frame="missile_blue_v.png",pos=Vec2(20,19),offset=Vec2(0,0),step=Vec2(5,5),isSolid=false,isHarmful=true,onCheckTile=false,direction=MovingDirection.toTop,}
    missile.fsm = Fsm:new(missile,nil,CommonStates.Move)
    Resh.playSound(Sound.missile)
    createRole(missile) 
    trigger.isLaunched=true
  end
  --drink后
  if (not trigger.isDrunken and bound.x == 36 and bound.y==15) then
   --- getStage():setGid(Gid.spike,Vec2(36,15))
    trigger.isDrunken=true
  end
  --天降agnail
  if (nil == trigger.isFallen and bound.x == 61) then
    trigger.isFallen = true
    Resh.playSound(Sound.fall)
    --  for i=1,4,1 do  
    local agnail1 = {type=RoleType.npc,frame="agnail_red1.png",pos=Vec2(62 ,9), step=Vec2(0.2,0.2),isSolid=false,direction=MovingDirection.toLeft, gravity=Vec2(0,-0.2),sound=Sound.sting}
    agnail1.fsm = Fsm:new(agnail1,nil,State10.Npc.AgnailArray)

    createRole(agnail1)

    local agnail2 = {type=RoleType.npc,frame="agnail_red1.png",pos=Vec2(63 ,9), step=Vec2(0.2,0.2),isSolid=false,direction=MovingDirection.toLeft, gravity=Vec2(0,-0.2),sound=Sound.sting}
    agnail2.fsm = Fsm:new(agnail2,nil,State10.Npc.AgnailArray)

    createRole(agnail2)

    local agnail3 = {type=RoleType.npc,frame="agnail_red1.png",pos=Vec2(64 ,9), step=Vec2(0.2,0.2),isSolid=false,direction=MovingDirection.toLeft, gravity=Vec2(0,-0.2),sound=Sound.sting}
    agnail3.fsm = Fsm:new(agnail3,nil,State10.Npc.AgnailArray)

    createRole(agnail3)

    local agnail4 = {type=RoleType.npc,frame="agnail_red1.png",pos=Vec2(65 ,9), step=Vec2(0.2,0.2),isSolid=false,direction=MovingDirection.toLeft, gravity=Vec2(0,-0.2),sound=Sound.sting}
    agnail4.fsm = Fsm:new(agnail4,nil,State10.Npc.AgnailArray)

    createRole(agnail4)
    --  end
  end

  if (bound.x >= 66) then
    Role[RoleIndex.reboundAgnail].ref:setAnimation(Animate.reboundAgnail)
    trigger.fsm:clearState()

  end

end

State10.RObject.SpikeTop = State:new()
State10.RObject.SpikeTop.execute = function(object)
  if Role[RoleIndex.bug].ref:isDead() then
    object.isDisposed = true
  end
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x == 12 and bound.y >= 14) then
    object.ref:setGravityOn()
  end
  if  object.allowCheck then
    object.getTarget(object,Role[RoleIndex.bug],function(range,target)
        -- 如果是玩家自己什么或子弹
        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = object.ref:checkObjectCollision(target.ref,false)
        if (isCollided) then
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,target,MessageType.collider)
          object.allowCheck = false
        end
      end)
  end
  if object.ref:getDistance().y > ScreenHeight * 2 then
    object.allowCheck = true
    object.ref.allowGravity = false
    --offset
    object.ref:spawn(object.pos,Vec2(0,16))
  end
end

State10.Npc.Bug = State:new()
State10.Npc.Bug.enter = function()
  Delay.reset(DelayIndex.common)
end
State10.Npc.Bug.execute = function(npc)
   npc.ref:showHints(Hints.bug)
  if npc.ref:isAlive()  and Delay.isTimeUp(DelayIndex.common,2.2) then
    NpcStates.WanderingX.execute(npc)
  end
end
State10.Npc.Bug.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.collider) then
    npc.ref:setFrame("bug_blue2.png")
    npc.ref.isSolid = true
    npc.ref.isHarmful=false
    npc.ref:allowPush(CollisionDirection.intersected)
    npc.ref:gotoHell(Sound.sting)
    npc.ref:stop()
  end
  if (telegram.message == MessageType.pushed) then
    npc.ref:gotPushed(telegram.sender.ref)
  end
end


State10.RObject.Key = State:new()
State10.RObject.Key.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.collider) then

    if (npc.tag == RoleIndex.keyBlue) then

      local missile = {type=RoleType.robject,frame="missile_red.png",pos=Vec2(0,13),offset=Vec2(0,20),step=Vec2(1,1),insetY=4,zorder=ZOrder.cover, allowCollision=CollisionDirection.atTop,onCheckTile=false,direction=MovingDirection.toRight,distance=Vec2(ScreenWidth+50,ScreenHeight),}
      missile.fsm = Fsm:new(missile,nil,CommonStates.Move)
      Resh.playSound(Sound.missile)
      createRole(missile)
    elseif (npc.tag == RoleIndex.keyRed) then
      getStage():setGid(Gid.spikeBottom,Vec2(13,12))

    end
    npc.isDisposed=true
  end
end

State10.Npc.Agnail = State:new()
State10.Npc.Agnail.execute = function(npc)
  npc.ref:showHints(Hints.slimePatroll)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x > 26 and bound.x < 32 and bound.y==17 and npc.ref.moveStep.x <=5.15) then
    npc.ref.moveStep = Vec2(npc.ref.moveStep.x + 0.12,npc.ref.moveStep.y +0.12)
  end

  if (not npc.isBreached and npc.ref.moveStep.x >=4.15) then
    local npcBound = npc.ref:getBoundSelf().tilePosition
    if (npcBound.x == 31 and npcBound.y == 17) then
      ActionExtend.corrupt(Vec2(32,17),1)
      getStage():removeTile(Vec2(32,17))
      npc.isBreached = true
    end
  end
  --已经冲破
  if (npc.isBreached) then
    npc.getTarget(npc,Role[RoleIndex.drink],function(range,target)
        -- 如果是玩家自己什么或子弹
        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = npc.ref:checkObjectCollision(target.ref,false)
        if (isCollided) then
          Resh.playSound(Sound.enigma)
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,npc,target,MessageType.collider)
          npc.isDisposed = true
        end
      end)
  end
  npc.ref:patrollingX(true)
end
State10.Npc.Agnail.onMessage = function(npc, telegram)
  if (telegram.sender.tag == 1) then
    npc.ref:setFrame("agnail_red2.png")
    npc.fsm:clearState()
    npc.ref:stop()
  end
end
State10.RObject.Drink = State:new()
State10.RObject.Drink.onMessage = function(object, telegram)
  --agnail 或者 player 全部销毁
  object.isDisposed = true
  Role[RoleIndex.board].ref:setAllowDragY(true)
  if (telegram.sender.tag == 1) then
    Role[RoleIndex.board].ref:registerText(RObjectText.drunkByPlayer)
  else
    Role[RoleIndex.board].ref:registerText(RObjectText.drunkByNpc)
    Ref.stage:promptAchieve(AchieveIndex.Drink,PopupInfo.achieve(AchieveIndex.Drink))
  end
  Role[RoleIndex.board].ref.isSolid = true
  Role[RoleIndex.board].ref:allowCollision(CollisionDirection.atTop)
end
State10.RObject.EightSpikes = State:new()
State10.RObject.EightSpikes.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x == 48 and   bound.y > 10 and bound.y<18) then
    object.ref:setGravityOn()
  end
  CommonStates.OutForDisposal.execute(object)
end
State10.Npc.AgnailArray = State:new()
State10.Npc.AgnailArray.execute = function(npc)
  if (Role[1].ref:isDead()) then
    npc.ref:setFrame("agnail_red2.png") 
    npc.fsm:clearState()
  end

  npc.getTarget(npc,Role[RoleIndex.pipe],function(range,target)
      -- 如果是玩家自己什么或子弹
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local isCollided = npc.ref:checkObjectCollision(target.ref,false)
      if (isCollided) then
        --【 如果是实体则
        if (target.ref.isSolid and target.except == nil) then
          npc.ref:checkObjectSide()
          npc.ref:checkFollowing()
        end

      end
    end)

  if (not npc.ref:inAir(false)) then
    CommonStates.Move.execute(npc)
  end
end
State10.RObject.Pipe = State:new()
State10.RObject.Pipe.execute = function(object)
  if (object.ref:getDistance().y > 100) then
    object.ref:setAllowDragY(false)
    object.fsm:clearState()
  end
end

State10.RObject.ShardOrange = State:new()
State10.RObject.ShardOrange.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if (bound.y >= 14 and bound.x > 68) then
    object.ref:spawn(Vec2(73,17),object.offset)
    object.ref:stopAnimation()
    getStage():setGidRange(Gid.spikeBottom,Vec2(69,17),Vec2(71,17))
    object.fsm:changeState(RObjectStates.Destiny)
  end
end

return State10
local Telegram = require("telegram")
local State = require("state")
local PlayerStates = { }

PlayerStates.Routine = State:new()
PlayerStates.Routine.execute = function(player)
  if player.ref:isDead()  then
    if not player.ref.isVisible then
       player.ref:closeDialog()
    else
      player.ref:showHints(PopupText.getDoneInfo())
    end
  end
  player.getOpponents(player,function(range,opponent)
      --projectTile
      if opponent.type == RoleType.projectTile   or 
        (opponent.isExcepted ~= nil and opponent.isExcepted) or
        range < RangeType.rangeThreatened  then 
        return
      end

      local isCollided = player.ref:checkObjectCollision(opponent.ref,false)
      if (isCollided) then
        --【第一步】如果是实体则
        if (opponent.ref.isSolid ) then
          player.ref:checkObjectSide()
          player.ref:checkFollowing()
        end

        --【第二步】玩家牺牲了退出
        if (player.ref:getBodyStatus() == BodyStatus.dead) then
          --player.ref:showHints()
          return
        end
        --【第三步】有害
        if (opponent.ref.isHarmful) then
          --根据sound判断
          if (opponent.sound ~= nil) then
            player.ref:gotoHell(opponent.sound)
          else
            player.ref:gotoHell(Sound.playerHurt)
          end
          --给npc发一个消息,改变frame
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,opponent,MessageType.hit)
          --给自己发一个消息，是哪个npc或object,注意最后一个参数是opponent 
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,player,MessageType.hit,opponent)
          return
        end
        --npc友好的
        if (opponent.type == RoleType.npc ) then
          --推箱子
          if (opponent.ref:hasPushes()) then
            MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,opponent,MessageType.pushed)
          end
        end

        if (opponent.type == RoleType.robject ) then

          --允许带走
          if (opponent.allowTaken) then
            player.ref:carryObject(opponent.ref)
            return
          end
          --标记没有allowNotify，发送对方robject
          if  opponent.allowNotify == nil or  opponent.allowNotify then
            MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,opponent,MessageType.collider)
          end
          
          --敲击,有敲击设置则发出,而且还要判断敲击的位置
          if (opponent.ref:hasKnocks(player.ref:getCollidedDirection())) then
            MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,opponent,MessageType.knocked)
          end
          --推
          if (opponent.ref:hasPushes()) then
            MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,opponent,MessageType.pushed)
          end
        end
      end
    end)


end

PlayerStates.Routine.onMessage = function(player, telegram)

  if (telegram.message == MessageType.sting and not player.ref:isDead()) then
    player.ref:gotoHell(Sound.sting)
  end

  return false
end
--玩家射出的子弹事件
PlayerStates.ProjectTile = State:new()
PlayerStates.ProjectTile.execute = function(object)

  local projectType =object.ref:getProjectType()
  --如果是子弹或跟踪
  if (projectType == ProjectType.object or  projectType == ProjectType.trace) then

    --循环每一个role，不包括自己和子弹
    object.getOpponents(object,function(range,opponent)
        -- 如果是玩家自己什么或子弹
        if (opponent.type == RoleType.player or 
          opponent.type == RoleType.projectTile or
          (opponent.isExcepted ~= nil and opponent.isExcepted )) then
          return
        end

        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = object.ref:checkObjectCollision(opponent.ref,false)
        if (isCollided) then
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,opponent,MessageType.gotShot)
           if (not object.ref:allowThrough()) then
            object.isDisposed = true
          end
        end
      end)
    --如果是一枪命中
  elseif (projectType == ProjectType.deadshot) then
    --【注意】有的子弹没有target，直接获取可能无法转换
    local tag = object.ref:getTarget():getTag()
    object.getTarget(object,Role[tag],function(range,target)
        -- 如果是玩家自己什么或子弹

        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = object.ref:checkObjectCollision(target.ref,false)
        if (isCollided) then
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,target,MessageType.gotShot)
          if (not object.ref:allowThrough()) then
            object.isDisposed = true
          end
        end
      end)
  end
  --生命周期
  if (object.ref:isDead()) then
    object.isDisposed = true
  end
end
return PlayerStates

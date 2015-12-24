ToggleButton = function (buttonId, iconId, initialStateOn) {
  this.buttonId = buttonId;
  this.stateOn = initialStateOn;
  this.iconId = iconId;
  this.redrawButton();
}

ToggleButton.prototype.isDisabled = function() {
  return $(this.buttonId).hasClass("disabled");
}

ToggleButton.prototype.isEnabled = function() {
  return !this.isButtonDisabled();
}

ToggleButton.prototype.isOn = function() {
  return this.stateOn;
}

ToggleButton.prototype.isOff = function() {
  return !this.isOn();
}

ToggleButton.prototype.enableDisableButton = function(enable) {
  if (enable) {
    if (this.isButtonDisabled()) {
      $(this.buttonId).removeClass("disabled");
    }
  } else if (this.isButtonEnabled()) {
    $(this.buttonId).addClass("disabled");
  }
}

ToggleButton.prototype.redrawButton = function() {
  if (this.isOn()) {
    $(this.iconId).removeClass("fa-toggle-off");
    $(this.iconId).addClass("fa-toggle-on");
  } else {
    $(this.iconId).removeClass("fa-toggle-on");
    $(this.iconId).addClass("fa-toggle-off");
  }
}

ToggleButton.prototype.onToggle = function() {
  this.stateOn = !this.stateOn;
  this.redrawButton();
}

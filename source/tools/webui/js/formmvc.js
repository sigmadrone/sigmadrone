FormView = function (formId, submitButtonId, cancelButtonId, statusId) {
  this.formId = formId;
  this.submitButtonId = submitButtonId;
  this.cancelButtonId = cancelButtonId;
  this.statusId = statusId;
  this.state = "STATE_WAITING_ON_DATA";

  this.enableDisableFormInputs(false);
  this.redrawControls();
}

FormView.prototype.isButtonDisabled = function(buttonId) {
  return $(this.formId).find(buttonId).hasClass("disabled");
}

FormView.prototype.isButtonEnabled = function(buttonId) {
  return !this.isButtonDisabled(buttonId);
}

FormView.prototype.enableDisableButton = function(buttonId, enable) {
  if (enable) {
    if (this.isButtonDisabled(buttonId)) {
      $(this.formId).find(buttonId).removeClass("disabled");
    }
  } else if (this.isButtonEnabled(buttonId)) {
    $(this.formId).find(buttonId).addClass("disabled");
  }
}

FormView.prototype.enableDisableFormInputs = function(enable) {
  $(this.formId + " :input").attr("disabled", !enable);
}

FormView.prototype.enableDisableSubmitButton = function(enable) {
  this.enableDisableButton(this.submitButtonId, enable);
}

FormView.prototype.enableDisableCancelButton = function(enable) {
  this.enableDisableButton(this.cancelButtonId, enable);
}

FormView.prototype.displayStatus = function(status) {
  $(this.statusId).css('color', 'grey');
  $(this.statusId).text(status);
}

FormView.prototype.displayError = function(error) {
  $(this.statusId).css('color', 'red');
  $(this.statusId).text(error);
}

FormView.prototype.redrawControls = function() {
  if (this.state == "STATE_WAITING_ON_DATA") {
    this.enableDisableSubmitButton(false);
    this.enableDisableCancelButton(false);
    this.displayStatus("Waiting for drone state...");
  } else if (this.state == "STATE_WAITING_ON_INPUT") {
    this.enableDisableSubmitButton(true);
    this.enableDisableCancelButton(false);
    this.displayStatus("");
  } else if (this.state == "STATE_DATA_INPUT") {
    this.enableDisableSubmitButton(true);
    this.enableDisableCancelButton(false);
    this.displayStatus("");
  } else if (this.state == "STATE_SUBMITTING_DATA") {
    this.enableDisableSubmitButton(false);
    this.enableDisableCancelButton(true);
    this.displayStatus("Applying changes..");
  } else if (this.state == "STATE_SUBMIT_FAILED") {
    this.enableDisableSubmitButton(false);
    this.enableDisableCancelButton(true);
    this.displayError("Failed to apply changes!");
  }
}

FormView.prototype.onUserInput = function() {
  if (this.state == "STATE_WAITING_ON_INPUT") {
    this.state = "STATE_DATA_INPUT";
    this.redrawControls();
    this.enableDisableCancelButton(true);
  }
  return true;
}

FormView.prototype.onSubmitButton = function() {
  $(this.formId).find(':submit').blur();
  if (this.state != "STATE_DATA_INPUT") {
    return true;
  }
  if (!$(this.formId)[0].checkValidity()) {
    return false;
  }
  this.state = "STATE_SUBMITTING_DATA";
  this.redrawControls();
  return true;
}

FormView.prototype.onCancelButton = function() {
  this.state = "STATE_WAITING_ON_DATA";
  this.enableDisableFormInputs(false);
  this.redrawControls();
}

FormView.prototype.onDataSubmitOK = function() {
  this.state = "STATE_WAITING_ON_DATA";
  this.enableDisableFormInputs(true);
  this.redrawControls();
}

FormView.prototype.onDataSubmitFail = function() {
  this.state = "STATE_SUBMIT_FAILED";
  this.redrawControls();
  this.state = "STATE_WAITING_ON_DATA";
}

FormView.prototype.onRedrawData = function(droneState) { // this method mus be extended
  this.enableDisableFormInputs(true);
  if (this.state != "STATE_WAITING_ON_DATA") {
    return false;
  }
  if (this.state == "STATE_WAITING_ON_DATA") {
    this.state = "STATE_WAITING_ON_INPUT";
  }
  this.redrawControls();
  return true;
}

function PidXYFormView(formId, submitButtonId, cancelButtonId, statusId) {
  FormView.call(this, formId, submitButtonId, cancelButtonId, statusId);
}
PidXYFormView.prototype = Object.create(FormView.prototype);
PidXYFormView.prototype.constructor = PidXYFormView;

PidXYFormView.prototype.onRedrawData = function(droneState) {
  if (!FormView.prototype.onRedrawData.call(this,droneState)) {
    return false;
  }
  $(this.formId)[0].elements['kp-xy'].value = droneState.kp;
  $(this.formId)[0].elements['ki-xy'].value = droneState.ki;
  $(this.formId)[0].elements['kd-xy'].value = droneState.kd;
  return true;
}


function PidZFormView(formId, submitButtonId, cancelButtonId, statusId) {
  FormView.call(this, formId, submitButtonId, cancelButtonId, statusId);
}
PidZFormView.prototype = Object.create(FormView.prototype);
PidZFormView.prototype.constructor = PidZFormView;

PidZFormView.prototype.onRedrawData = function(droneState) {
  if (!FormView.prototype.onRedrawData.call(this,droneState)) {
    return false;
  }
  $(this.formId)[0].elements['kp-z'].value = droneState.yaw_kp;
  $(this.formId)[0].elements['ki-z'].value = droneState.yaw_ki;
  $(this.formId)[0].elements['kd-z'].value = droneState.yaw_kd;
  return true;
}
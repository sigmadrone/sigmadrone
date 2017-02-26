FormView = function (formId, submitButtonId, cancelButtonId, statusId, submitStatusMsg, errorStatusMsg) {
  this.formId = formId;
  this.submitButtonId = submitButtonId;
  this.cancelButtonId = cancelButtonId;
  this.statusId = statusId;
  this.state = "STATE_WAITING_ON_DATA";
  this.submitStatusMsg = submitStatusMsg;
  if (null == this.submitStatusMsg) {
    this.submitStatusMsg = "Applying changes...";
  }
  this.errorStatusMsg = errorStatusMsg;
  if (null == this.errorStatusMsg) {
    this.errorStatusMsg = "Failed to apply changes!"
  }

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
    this.enableDisableSubmitButton(true);
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
    this.displayStatus(this.submitStatusMsg);
  } else if (this.state == "STATE_SUBMIT_FAILED") {
    this.enableDisableSubmitButton(false);
    this.enableDisableCancelButton(true);
    this.displayError(this.errorStatusMsg);
  }
  $(this.formId).find(this.submitButtonId).blur();
  $(this.formId).find(this.cancelButtonId).blur();
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
  $(this.formId).find(this.submitButtonId).blur();
  if (this.state != "STATE_DATA_INPUT") {
    return true;
  }
  if (!$(this.formId)[0].checkValidity()) {
    return false;
  }

  $(this.formId + " *").filter(':input').each(function(){
    if (this.type == "number") {
      this.value = String(Number(this.value));
    }
  });

  this.state = "STATE_SUBMITTING_DATA";
  this.redrawControls();
  return true;
}

FormView.prototype.onCancelButton = function() {
  if (this.isButtonDisabled(this.cancelButtonId)) {
    $(this.formId).find(this.cancelButtonId).blur();
    return;
  }
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
  if (this.state != "STATE_WAITING_ON_DATA" && this.state != "STATE_WAITING_ON_INPUT") {
    return false;
  }
  if (this.state == "STATE_WAITING_ON_DATA") {
    this.state = "STATE_WAITING_ON_INPUT";
  }
  this.redrawControls();
  $(this.formId).find(this.submitButtonId).blur();
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

function PidAltitudeFormView(formId, submitButtonId, cancelButtonId, statusId) {
  FormView.call(this, formId, submitButtonId, cancelButtonId, statusId);
}
PidAltitudeFormView.prototype = Object.create(FormView.prototype);
PidAltitudeFormView.prototype.constructor = PidAltitudeFormView;

PidAltitudeFormView.prototype.onRedrawData = function(droneState) {
  if (!FormView.prototype.onRedrawData.call(this,droneState)) {
    return false;
  }
  $(this.formId)[0].elements['kp-altitude'].value = droneState.altitude_kp;
  $(this.formId)[0].elements['ki-altitude'].value = droneState.altitude_ki;
  $(this.formId)[0].elements['kd-altitude'].value = droneState.altitude_kd;
  $(this.formId)[0].elements['alt-correction-period'].value = droneState.altitude_correction_period;
  return true;
}

function PidGyroFormView(formId, submitButtonId, cancelButtonId, statusId) {
  FormView.call(this, formId, submitButtonId, cancelButtonId, statusId);
}
PidGyroFormView.prototype = Object.create(FormView.prototype);
PidGyroFormView.prototype.constructor = PidGyroFormView;

PidGyroFormView.prototype.onRedrawData = function(droneState) {
  if (!FormView.prototype.onRedrawData.call(this,droneState)) {
    return false;
  }
  $(this.formId)[0].elements['kp-gyro'].value = droneState.gyro_drift_kp;
  $(this.formId)[0].elements['ki-gyro'].value = droneState.gyro_drift_ki;
  $(this.formId)[0].elements['kd-gyro'].value = droneState.gyro_drift_kd;
  return true;
}

function FlightCtlFormView(formId, submitButtonId, cancelButtonId, statusId) {
  FormView.call(this, formId, submitButtonId, cancelButtonId, statusId);
}
FlightCtlFormView.prototype = Object.create(FormView.prototype);
FlightCtlFormView.prototype.constructor = FlightCtlFormView;

FlightCtlFormView.prototype.onRedrawData = function(droneState) {
  if (!FormView.prototype.onRedrawData.call(this,droneState)) {
    return false;
  }
  $(this.formId)[0].elements['torq-bias-roll'].value = droneState.roll_bias;
  $(this.formId)[0].elements['torq-bias-pitch'].value = droneState.pitch_bias;
  $(this.formId)[0].elements['torq-bias-yaw'].value = droneState.yaw_bias;
  $(this.formId)[0].elements['torq-bias-yaw'].value = droneState.yaw_bias;
  $(this.formId)[0].elements['track-magnetometer'].checked = droneState.track_magnetometer;
  $(this.formId)[0].elements['external-gyro'].checked = droneState.ext_gyro_enabled;
  $(this.formId)[0].elements['yaw-throttle-factor'].value = droneState.yaw_throttle_factor;

  if (droneState.flight_mode == 1) {
    $(this.formId)[0].elements['flight-mode-id'].value = "flight-mode-altitude";
  } else {
    $(this.formId)[0].elements['flight-mode-id'].value = "flight-mode-auto-level";
  }
  return true;
}

function SensorsCfgFormView(formId, submitButtonId, cancelButtonId, statusId) {
  FormView.call(this, formId, submitButtonId, cancelButtonId, statusId);
}
SensorsCfgFormView.prototype = Object.create(FormView.prototype);
SensorsCfgFormView.prototype.constructor = SensorsCfgFormView;

SensorsCfgFormView.prototype.onRedrawData = function(droneState) {
  if (!FormView.prototype.onRedrawData.call(this,droneState)) {
    return false;
  }
  $(this.formId)[0].elements['acc-bias-x'].value = droneState.accel_adjustment[0];
  $(this.formId)[0].elements['acc-bias-y'].value = droneState.accel_adjustment[1];
  $(this.formId)[0].elements['acc-bias-z'].value = droneState.accel_adjustment[2];
  $(this.formId)[0].elements['acc-corr-period'].value = droneState.accelerometer_correction_period;
  $(this.formId)[0].elements['gyro-factor'].value = droneState.gyro_factor;
  return true;
}

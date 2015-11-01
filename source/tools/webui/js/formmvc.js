FormView = function (formId, submitButtonId, cancelButtonId, statusId) {
  this.formId = formId;
  this.submitButtonId = submitButtonId;
  this.cancelButtonId = cancelButtonId;
  this.statusId = statusId;
  this.state = "STATE_WAITING_ON_DATA";

  this.isButtonDisabled = function(buttonId) {
    return $(this.formId).find(buttonId).hasClass("disabled");
  }

  this.isButtonEnabled = function(buttonId) {
    return !this.isButtonDisabled(buttonId);
  }

  this.enableDisableButton = function(buttonId, enable) {
    if (enable) {
      if (this.isButtonDisabled(buttonId)) {
        $(this.formId).find(buttonId).removeClass("disabled");
      }
    } else if (this.isButtonEnabled(buttonId)) {
      $(this.formId).find(buttonId).addClass("disabled");
    }
  }

  this.enableDisableFormInputs = function(enable) {
    $(this.formId + " :input").attr("disabled", !enable);
  }

  this.enableDisableSubmitButton = function(enable) {
    this.enableDisableButton(submitButtonId, enable);
  }

  this.enableDisableCancelButton = function(enable) {
    this.enableDisableButton(cancelButtonId, enable);
  }

  this.displayStatus = function(status) {
    $(this.statusId).css('color', 'grey');
    $(this.statusId).text(status);
  }
  
  this.displayError = function(error) {
    $(this.statusId).css('color', 'red');
    $(this.statusId).text(error);
  }

  this.redrawControls = function() {
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

  this.onUserInput = function() {
    if (this.state == "STATE_WAITING_ON_INPUT") {
      this.state = "STATE_DATA_INPUT";
      this.redrawControls();
      this.enableDisableCancelButton(true);
    }
    return true;
  }
  
  this.onSubmitButton = function() {
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
  
  this.onCancelButton = function() {
    this.state = "STATE_WAITING_ON_DATA";
    this.enableDisableFormInputs(false);
    this.redrawControls();
  }
  
  this.onDataSubmitOK = function() {
    this.state = "STATE_WAITING_ON_DATA";
    this.enableDisableFormInputs(true);
    this.redrawControls();
  }
  
  this.onDataSubmitFail = function() {
    this.state = "STATE_SUBMIT_FAILED";
    this.redrawControls();
    this.state = "STATE_WAITING_ON_DATA";
  }
  
  this.onRedrawData = function(droneState) {
    this.enableDisableFormInputs(true);
    if (this.state != "STATE_WAITING_ON_DATA") {
      return false;
    }
    if (this.state == "STATE_WAITING_ON_DATA") {
      this.state = "STATE_WAITING_ON_INPUT";
    }
    this.redrawControls();
    return true; // method must be extended
  }

  this.enableDisableFormInputs(false);
  this.redrawControls();
}
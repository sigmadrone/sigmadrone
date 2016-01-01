function assert(condition, message) {
    if (!condition) {
        throw message || "Assertion failed";
    }
}

VisChart2d = function (containerId, statusId, chartTitle, ylabels, rangeMin, rangeMax, ylabelsRight) {
  this.dataset = new vis.DataSet({type: {start: 'Number', end: 'Number' }});
  this.statusId = statusId;

  // Create the groups, number of groups is controlled by the size of the ylabels
  // array
  this.groups = new vis.DataSet();
  for (var i = 0; i < ylabels.length; ++i) {
    this.groups.add({
        id: i,
        content: ylabels[i],
        options: {
          drawPoints: {
            style: 'circle',
            size: 3
          }
      }
    });
  }
  this.numLeftYAxis = ylabels.length;

  if (null != ylabelsRight) {
    for (var i = 0; i < ylabelsRight.length; ++i) {
      this.groups.add({
        id: this.numLeftYAxis + i,
        content: ylabelsRight[i],
        options: {
          yAxisOrientation: 'right',
          drawPoints: {
            style: 'square',
            size: 3
          }
        }
      });
    }
    this.numRightYAxis = ylabelsRight.length;
  } else {
    this.numRightYAxis = 0;
  }


  var options = {
    start: 0, //vis.moment().add(-30, 'seconds'), // changed so its faster
    end: 1000, //vis.moment(),
    showCurrentTime: true,
    dataAxis: {
      left: {
        range: {
          min:rangeMin, max: rangeMax
        },
        format: function(value) {
          return value.toFixed(2);
        },
        /*title: {
          text: chartTitle
        }*/
      }
    },
    sampling: true,
    legend: {left:{position:"top-left"}},
    //clickToUse:true,
    showMajorLabels:false,
    showMinorLabels:true,
    height: '400px',
    interpolation: true,
  };

  this.graph2d = new vis.Graph2d(containerId, this.dataset, this.groups, options);

  this.firstIteration = 0;
  this.dataset.clear();
  this.newData = [];
  this.dataId = 0;
}

VisChart2d.prototype.reset = function() {
  this.firstIteration = 0;
  this.dataset.clear();
  this.newData = [];
  this.dataId = 0;
}

VisChart2d.prototype.update = function(samples, iteration) {
  assert(samples.length <= this.groups.length);
  assert(this.firstIteration <= iteration);
  if (this.firstIteration == 0) {
    this.updateStatus("COLLECT");
    this.firstIteration = iteration;
  }
  for (var i = 0; i < this.numLeftYAxis; ++i) {
    this.newData.push(
      {id: this.dataId++, x: iteration-this.firstIteration, y:samples[i], group: i}
    );
  }

  for (var i = this.numLeftYAxis; i < samples.length; ++i) {
    this.newData.push(
      {id: this.dataId++, x: iteration-this.firstIteration, y:samples[i], group: i}
    );
  }

  //var now = iteration - this.firstIteration;//vis.moment();
}

VisChart2d.prototype.draw = function() {
  this.updateStatus("DRAWING");
  this.dataset.add(this.newData);
  var now = this.newData.length;
  var range = this.graph2d.getWindow();
  var interval = range.end - range.start;
  /*if (interval > 1000) {
    interval = 1000;
  }*/
  this.graph2d.setWindow(now - interval, now, {animation: false});
  this.updateStatus("DONE");
}

VisChart2d.prototype.updateStatus = function(state) {
  if (null != this.statusId) {
    if (state == "RESET") {
      $(this.statusId).text("");
    } else if (state == "COLLECT") {
      $(this.statusId).text("Collecting data...");
    } else if (state == "DRAWING") {
      $(this.statusId).text("Drawing...");
    } else if (state == "DONE") {
      $(this.statusId).text("Collected " + this.dataId + " samples");
    }
  }
}

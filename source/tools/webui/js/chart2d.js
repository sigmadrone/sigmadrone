function assert(condition, message) {
    if (!condition) {
        throw message || "Assertion failed";
    }
}

VisChart2d = function (containerId, statusId, chartTitle, ylabels, rangeMin, rangeMax, ylabelsRight) {
  this.dataset = new vis.DataSet({type: {start: 'ISODate', end: 'ISODate' }});
  this.statusId = statusId;
  this.rangeMin = rangeMin;
  this.rangeMax = rangeMax;

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

  this.graph2d = new vis.Graph2d(containerId, this.dataset, this.groups, this.getOptions());

  this.reset();
}

VisChart2d.prototype.getOptions = function() {
  return {
    start: vis.moment(),
    end: vis.moment() + 60 * 1000,
    showCurrentTime: true,
    dataAxis: {
      left: {
        range: {
          min: this.rangeMin, max: this.rangeMax
        },
        format: function(value) {
          return value.toFixed(2);
        },
        //title: {
        //  text: chartTitle
        //}
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
}

VisChart2d.prototype.reset = function() {
  this.lastIteration = this.firstIteration = -1;
  this.dataset.clear();
  this.newData = [];
  this.dataId = 0;
  this.beginTimeLine = 0;
  this.graph2d.setOptions(this.getOptions());
}

VisChart2d.prototype.update = function(samples, iteration) {
  assert(this.firstIteration <= iteration);

  if (this.firstIteration < 0) {
    this.updateStatus("COLLECT");
    this.firstIteration = iteration;
    this.beginTimeLine = vis.moment();
  }

  var xData = this.beginTimeLine + (iteration - this.firstIteration) * 5.8;
  for (var i = 0; i < this.numLeftYAxis; ++i) {
    this.newData.push(
      {id: this.dataId++, x: xData, y:samples[i], group: i}
    );
  }

  for (var i = this.numLeftYAxis; i < samples.length && i < this.groups.length; ++i) {
    this.newData.push(
      {id: this.dataId++, x: xData, y: samples[i], group: i}
    );
  }

  this.lastIteration = iteration;
}

VisChart2d.prototype.draw = function() {
  this.updateStatus("DRAWING");
  if (this.newData.length > 0) {
    this.graph2d.setWindow(
      this.beginTimeLine,
      this.newData[this.newData.length-1].x,
      {animation: false});
    this.dataset.add(this.newData);
  }
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
      $(this.statusId).text("Collected " + this.dataId / this.groups.length + " samples");
    }
  }
}

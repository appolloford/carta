/**
 * Utility for enumerating unique html identifiers and adding them to the
 * page.
 */

qx.Class.define("skel.widgets.TestID", {
    type : "static",
    statics : {
        
        /**
         * Adds an 'id' attribute to the widget's html div.
         * @param widget {qx.ui.basic.Widget}.
         * @param testId {String} a unique id for locating the html element.
         */
        addTestId : function( widget, testId ){
            //Ids must be unique in the app so only use the id if we
            //have not already done so.
            if ( skel.widgets.TestID.widgetIds.indexOf( testId ) < 0 ){
                //Testing Id
                widget.addListener("appear", function() {
                    var container = this.getContentElement().getDomElement();
                    container.id = testId;
                }, widget );
                skel.widgets.TestID.widgetIds.push( testId );
            }
        },
        
        widgetIds : [],
        
        //Only ids used for uniquely identifying html elements should go between
        //the TESTIDS_START and TESTIDS_END tags.
        TESTIDS_START : null,
        COLOR_MAP_BUTTON : "showColorMapDialog",
        HISTOGRAM_BUTTON : "showHistogramDialog",
        HISTOGRAM_BIN_COUNT_CHECK : "showHistogramBinCount",
        HISTOGRAM_BIN_COUNT_INPUT : "histogramBinCountTextField",
        HISTOGRAM_BIN_COUNT_SLIDER : "histogramBinCountSlider",
        SHOW_POPUP_BUTTON : "popupDisplayWindow",
        TESTIDS_END : null
    }

});
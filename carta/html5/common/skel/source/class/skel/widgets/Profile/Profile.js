/**
 * Displays a profile and controls for customizing it.
 */
/*global mImport */
/*******************************************************************************
 * @ignore( mImport)
 ******************************************************************************/

qx.Class.define("skel.widgets.Profile.Profile", {
    extend : qx.ui.core.Widget, 

    /**
     * Constructor.
     */
    construct : function(  ) {
        this.base(arguments);
        this.m_connector = mImport("connector");
        this._init();
    },

    members : {
        
        /**
         * Initializes the UI.
         */
        _init : function( ) {
            this._setLayout(new qx.ui.layout.Grow());
            this.m_content = new qx.ui.container.Composite();
            this._add( this.m_content );
            this.m_content.setLayout(new qx.ui.layout.VBox());
            
            this._initMain();
            this._initControls();
        },
       
        /**
         * Initializes the profile settings.
         */
        _initControls : function(){
            this.m_settingsContainer = new skel.widgets.Profile.Settings();
        },
        
        
        /**
         * Initializes the menu for setting the visibility of individual profile
         * settings and the main graph.
         */
        _initMain : function(){
            this.m_mainComposite = new qx.ui.container.Composite();
            this.m_mainComposite.setLayout( new qx.ui.layout.VBox(2));
            this.m_mainComposite.set ({
                minWidth : this.m_MIN_DIM,
                minHeight : this.m_MIN_DIM,
                allowGrowX: true,
                allowGrowY: true
            });
            
            this.m_content.add( this.m_mainComposite, {flex:1});
        },
        
        
        /**
         * Initialize the profile view.
         */
        _initView : function(){
            if (this.m_view === null) {
                this.m_view = new skel.boundWidgets.View.DragView(this.m_id);
                this.m_fitOverlay = new skel.widgets.Profile.FitOverlay();
                this.m_view.setOverlayWidget( this.m_fitOverlay );
                this.m_view.setAllowGrowX( true );
                this.m_view.setAllowGrowY( true );
                this.m_view.setMinHeight(this.m_MIN_DIM);
                this.m_view.setMinWidth(this.m_MIN_DIM);
                if ( this.m_mainComposite.indexOf( this.m_view) < 0 ){
                    this.m_mainComposite.add( this.m_view, {flex:1} );
                }
            }
        },
        
        
        /**
         * Add or remove the control settings.
         * @param visible {boolean} - true if the control settings should be visible;
         *      false otherwise.
         */
        _layoutControls : function( ){
            if(this.m_showSettings){
                //Make sure the settings container is visible.
                if ( this.m_content.indexOf( this.m_settingsContainer ) < 0 ){
                    this.m_content.add( this.m_settingsContainer );
                }
            }
            else {
                if ( this.m_content.indexOf( this.m_settingsContainer ) >= 0 ){
                    this.m_content.remove( this.m_settingsContainer );
                }
            }
        },
        
        /**
         * Callback for updates of server-side fit preferences.
         */
        _profileCB : function(){
            var val = this.m_sharedVar.get();
            if ( val ){
                try {
                    var profilePrefs = JSON.parse( val );
                    this.m_fitOverlay.setManual( profilePrefs.showGuesses );
                    this.m_settingsContainer.prefUpdate( profilePrefs );
                }
                catch( err ){
                    console.log( "Could not parse: "+val+" error: "+err );
                }
            }
        },
        
        /**
         * Register to receive updates to server-side fit preference
         * variables.
         */
        _register : function(){
            var path = skel.widgets.Path.getInstance();
            this.m_sharedVar = this.m_connector.getSharedVar( this.m_id );
            this.m_sharedVar.addCB( this._profileCB.bind( this));
            this._profileCB();
        },
        
        /**
         * Set the server side id of this profiler.
         * @param controlId {String} the server side id of the object that produced this profile.
         */
        setId : function( controlId ){
            this.m_id = controlId;
            this.m_settingsContainer.setId( controlId );
            this._initView();
            this._register();
        },
        
        /**
         * Set whether or not manual initially guesses should be shown.
         * @param show {boolean} - whether or not controls for manual initial fit
         *      fit guesses should be shown.
         */
        setManual : function( show ){
            this.m_fitOverlay.setManual( show );
        },

        
        /**
         * Show or hide the profile settings.
         * @param visible {boolean} if the settings should be shown; false otherwise.
         */
        showHideSettings : function( visible ){
            this.m_showSettings = visible;
            this._layoutControls( );
        },
        
        m_content : null,
        m_fitOverlay : null,
        m_mainComposite : null,
        m_settingsContainer : null,
        m_showSettings : null,
        m_MIN_DIM : 195,
        m_id : null,
        m_sharedVar : null,
        m_connector : null,
        
        m_view : null
    }


});
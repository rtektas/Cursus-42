'use strict';

var currentTab   = 0;
var splitOn      = false;
var sshRunning   = false, sshTimer = null;
var sshStats     = {att:0, hit:0, fail:0};
var CURRENT_USER = null;
var pongStarted  = false;
var pongMini     = null;

/* FIX: SEC_IDS était manquant — toggleSplit crashait silencieusement */
var SEC_IDS = ['sec0', 'sec1', 'sec2', 'sec3', 'sec4', 'sec5', 'sec6'];

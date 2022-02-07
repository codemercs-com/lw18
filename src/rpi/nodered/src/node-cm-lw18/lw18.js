module.exports = function (RED) {
	//I2C addresses
	const LW18_DEFAULT_I2C = 0x20;
	const LW18_BROADCAST_I2C = 0x00;

	//I2C register
	const LW18_REG_COMMAND = 0x00;  //R/W : 1Byte ; Command
	const LW18_REG_PWM16 = 0x01;  //R/W : 4Byte ; 2x 16 bit PWM
	const LW18_REG_PWM8 = 0x02;  //W : 2Byte ; 2x 8 bit PWM (log)
	const LW18_REG_PERIODE = 0x03;  //W : 4Byte ; 2x 16 bit frequency
	const LW18_REG_SIGNATURE = 0xF0;  //R : 6Byte ; Signature
	const LW18_REG_SETADDR = 0xFE;  //W : 2Byte ; change I2C address

	//Bits for COMMAND register
	const LW18_MODE_SYNC_RESET = 0x00;
	const LW18_MODE_SYNC_ENABLE = 0x01;
	const LW18_MODE_LINMAP = 0x02;
	const LW18_PWM_TO_FLASH = 0x80;
	const LW18_SET_BROADCAST = 0x0A;

	//Frequency values
	const LW18_FREQ_DEFAULT = 48000000; //Hz -> 48 MHz
	const LW18_FREQ_1 = 730;      //z
	const LW18_MAX_PERIODE = 0xFFFF;


	"use strict";
	var I2C = require("i2c-bus");

	function WriteCommand(n) {
		RED.nodes.createNode(this, n);

		this.busno = isNaN(parseInt(n.busno)) ? 1 : parseInt(n.busno);	//I2C bus no. (raspi use 1 as defalut)
		this.address = parseInt(n.address);			//7bit I2C address
		this.cmd_value = n.cmd_value;				//Output value for PWM1 (0...65535)
		var node = this;
		node.port = I2C.openSync(node.busno);

		node.on("input", function (msg) {
			var address = LW18_DEFAULT_I2C;
			address = parseInt(node.address, 16);	//Convert DEC address from string into HEX 

			if (isNaN(address))
				address = LW18_DEFAULT_I2C;

			var cmd_value = node.cmd_value;
			var cmd = 0x00;

			//Get valid DALI address
			if (cmd_value == 0) cmd = LW18_MODE_SYNC_RESET;
			if (cmd_value == 1) cmd = LW18_MODE_SYNC_ENABLE;
			if (cmd_value == 2) cmd = LW18_MODE_LINMAP;
			if (cmd_value == 3) cmd = LW18_PWM_TO_FLASH;
			if (cmd_value == 4) cmd = LW18_SET_BROADCAST;

			if (isNaN(address)) {
				this.status({ fill: "red", shape: "ring", text: "Address (" + address + ") value is missing or incorrect" });
				return;
			}
			else {
				this.status({});
			}

			try {
				var report = Buffer.alloc(1);
				report[0] = cmd;
				node.port.writeI2cBlock(address, LW18_REG_COMMAND, report.length, report, function (err) {
					if (err) {
						node.error(err, msg);
					}
					else {
						node.send(msg);
					}
				});
			}
			catch (err) {
				this.error(err, msg);
			}
		});

		node.on("close", function () {
			node.port.closeSync();
		});
	}

	//Read command register
	function ReadCommand(n) {
		RED.nodes.createNode(this, n);

		this.busno = isNaN(parseInt(n.busno)) ? 1 : parseInt(n.busno);	//I2C bus no. (raspi use 1 as defalut)
		this.address = parseInt(n.address);			//7bit I2C address
		var node = this;
		node.port = I2C.openSync(node.busno);

		node.on("input", function (msg) {
			var address = LW18_DEFAULT_I2C;
			address = parseInt(node.address, 16);	//Convert DEC address from string into HEX 

			if (isNaN(address))
				address = LW18_DEFAULT_I2C;
			try {
				var report = Buffer.alloc(1);
				node.port.readI2cBlock(address, LW18_REG_COMMAND, report.length, report, function (err, size, res) {
					if (err) {
						node.error(err, msg);
					} else {
						msg = Object.assign({}, msg);
						msg.address = address;
						msg.command = LW18_REG_COMMAND;
						msg.payload = res[0];
						msg.size = size;
						node.send(msg);
					}
				});
			}
			catch (err) {
				this.error(err, msg);
			}
		});

		node.on("close", function () {
			node.port.closeSync();
		});
	}

	// Write 16bit PWM value
	function WritePwm16(n) {
		RED.nodes.createNode(this, n);

		this.busno = isNaN(parseInt(n.busno)) ? 1 : parseInt(n.busno);	//I2C bus no. (raspi use 1 as defalut)
		this.address = parseInt(n.address);			//7bit I2C address
		this.pwm1_value = n.pwm1_value;				//Output value for PWM1 (0...65535)
		this.pwm2_value = n.pwm2_value;				//Output value for PWM2 (0...65535)
		var node = this;
		node.port = I2C.openSync(node.busno);

		node.on("input", function (msg) {
			var address = LW18_DEFAULT_I2C;
			address = parseInt(node.address, 16);	//Convert DEC address from string into HEX 

			if (isNaN(address))
				address = LW18_DEFAULT_I2C;

			var pwm1_value = node.pwm1_value;
			var pwm2_value = node.pwm2_value;

			if (isNaN(address)) {
				this.status({ fill: "red", shape: "ring", text: "Address (" + address + ") value is missing or incorrect" });
				return;
			}
			else if (isNaN(pwm1_value)) {
				this.status({ fill: "red", shape: "ring", text: "PWM 1 (" + pwm1_value + ") value is missing or incorrect" });
				return;
			}
			else if (isNaN(pwm2_value)) {
				this.status({ fill: "red", shape: "ring", text: "PWM 2 (" + pwm2_value + ") value is missing or incorrect" });
				return;
			}
			else {
				this.status({});
			}

			try {
				var report = Buffer.alloc(4);
				report[0] = (pwm1_value & 0x00FF);
				report[1] = ((pwm1_value & 0xFF00) >> 8);
				report[2] = (pwm2_value & 0x00FF);
				report[3] = ((pwm2_value & 0xFF00) >> 8);

				node.port.writeI2cBlock(address, LW18_REG_PWM16, report.length, report, function (err) {
					if (err) {
						node.error(err, msg);
					}
					else {
						node.send(msg);
					}
				});
			}
			catch (err) {
				this.error(err, msg);
			}
		});

		node.on("close", function () {
			node.port.closeSync();
		});
	}

	//Read 16Bit PWM (8 bit not readable)
	function ReadPwm16(n) {
		RED.nodes.createNode(this, n);

		this.busno = isNaN(parseInt(n.busno)) ? 1 : parseInt(n.busno);	//I2C bus no. (raspi use 1 as defalut)
		this.address = parseInt(n.address);			//7bit I2C address
		var node = this;
		node.port = I2C.openSync(node.busno);

		node.on("input", function (msg) {
			var address = LW18_DEFAULT_I2C;
			address = parseInt(node.address, 16);	//Convert DEC address from string into HEX 

			if (isNaN(address))
				address = LW18_DEFAULT_I2C;

			try {

				var report = Buffer.alloc(4);
				node.port.readI2cBlock(address, LW18_REG_PWM16, report.length, report, function (err, size, res) {
					if (err) {
						node.error(err, msg);
					} else {
						msg = Object.assign({}, msg);
						msg.address = address;
						msg.command = LW18_REG_COMMAND;
						msg.payload = {
							pwm1: (res[0] | (res[1] << 8)),
							pwm2: (res[2] | (res[3] << 8))
						}
						msg.size = size;
						node.send(msg);
					}
				});
			}
			catch (err) {
				this.error(err, msg);
			}
		});

		node.on("close", function () {
			node.port.closeSync();
		});
	}

	//Write 8Bit PWM (Only write possible)
	function WritePwm8(n) {
		RED.nodes.createNode(this, n);

		this.busno = isNaN(parseInt(n.busno)) ? 1 : parseInt(n.busno);	//I2C bus no. (raspi use 1 as defalut)
		this.address = parseInt(n.address);			//7bit I2C address
		this.pwm1_value = n.pwm1_value;				//Output value for PWM1 (0...255)
		this.pwm2_value = n.pwm2_value;				//Output value for PWM2 (0...255)
		var node = this;
		node.port = I2C.openSync(node.busno);

		node.on("input", function (msg) {
			var address = LW18_DEFAULT_I2C;
			address = parseInt(node.address, 16);	//Convert DEC address from string into HEX 

			if (isNaN(address))
				address = LW18_DEFAULT_I2C;

			var pwm1_value = node.pwm1_value;
			var pwm2_value = node.pwm2_value;

			if (isNaN(address)) {
				this.status({ fill: "red", shape: "ring", text: "Address (" + address + ") value is missing or incorrect" });
				return;
			}
			else if (isNaN(pwm1_value)) {
				this.status({ fill: "red", shape: "ring", text: "PWM 1 (" + pwm1_value + ") value is missing or incorrect" });
				return;
			}
			else if (isNaN(pwm2_value)) {
				this.status({ fill: "red", shape: "ring", text: "PWM 2 (" + pwm2_value + ") value is missing or incorrect" });
				return;
			}
			else {
				this.status({});
			}

			try {
				var report = Buffer.alloc(2);
				report[0] = (pwm1_value & 0xFF);
				report[1] = (pwm2_value & 0xFF);

				node.port.writeI2cBlock(address, LW18_REG_PWM8, report.length, report, function (err) {
					if (err) {
						node.error(err, msg);
					}
					else {
						node.send(msg);
					}
				});
			}
			catch (err) {
				this.error(err, msg);
			}
		});

		node.on("close", function () {
			node.port.closeSync();
		});
	}

	function WriteFrequency(n) {
		RED.nodes.createNode(this, n);

		this.busno = isNaN(parseInt(n.busno)) ? 1 : parseInt(n.busno);	//I2C bus no. (raspi use 1 as defalut)
		this.address = parseInt(n.address);			//7bit I2C address
		this.pwm1_value = n.pwm1_value;				//Frequncy value for PWM1 (0...65535)
		this.pwm2_value = n.pwm2_value;				//Frequncy value for PWM2 (0...65535)
		var node = this;
		node.port = I2C.openSync(node.busno);

		node.on("input", function (msg) {
			var address = LW18_DEFAULT_I2C;
			address = parseInt(node.address, 16);	//Convert DEC address from string into HEX 

			if (isNaN(address))
				address = LW18_DEFAULT_I2C;

			var pwm1_value = node.pwm1_value;
			var pwm2_value = node.pwm2_value;

			if (isNaN(address)) {
				this.status({ fill: "red", shape: "ring", text: "Address (" + address + ") value is missing or incorrect" });
				return;
			}
			else if (isNaN(pwm1_value)) {
				this.status({ fill: "red", shape: "ring", text: "FREQ 1 (" + pwm1_value + ") value is missing or incorrect" });
				return;
			}
			else if (isNaN(pwm2_value)) {
				this.status({ fill: "red", shape: "ring", text: "FREQ 2 (" + pwm2_value + ") value is missing or incorrect" });
				return;
			}
			else {
				this.status({});
			}

			try {
				var report = Buffer.alloc(4);
				report[0] = (pwm1_value & 0x00FF);
				report[1] = ((pwm1_value & 0xFF00) >> 8);
				report[2] = (pwm2_value & 0x00FF);
				report[3] = ((pwm2_value & 0xFF00) >> 8);

				node.port.writeI2cBlock(address, LW18_REG_PERIODE, report.length, report, function (err) {
					if (err) {
						node.error(err, msg);
					}
					else {
						node.send(msg);
					}
				});
			}
			catch (err) {
				this.error(err, msg);
			}
		});

		node.on("close", function () {
			node.port.closeSync();
		});
	}

	RED.nodes.registerType("set cmd", WriteCommand);
	RED.nodes.registerType("get cmd", ReadCommand);
	RED.nodes.registerType("set pwm16", WritePwm16);
	RED.nodes.registerType("get pwm16", ReadPwm16);
	RED.nodes.registerType("set pwm8", WritePwm8);
	RED.nodes.registerType("set freq", WriteFrequency);
}
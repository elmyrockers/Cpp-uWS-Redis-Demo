document.addEventListener( 'alpine:init',()=>{
	Alpine.data( 'app',()=>({
		wsClient: {},
		onlineUsers: [],
		typingUsers: [],
		colors: {},
		messages: [],
		input: "",
		isTyping: false,
		typingTimeoutID: 0,
		
		init(){
			this.wsConnect();
		},
		typingList(){
			const usernames = this.typingUsers.map(username =>{
				return username.charAt(0).toUpperCase() + username.slice(1);
			});
			const lastUser = usernames.at(-1);
			const otherUsers = usernames.slice(0, -1).join(', ');
			
			let list = '';
			if ( otherUsers ) list = `${otherUsers} and `;
			list += lastUser;

			return list;
		},
		formatTime( timestamp ) {
			// Convert timestamp to datetime
				const ms = Number(timestamp)*1000;
				const date = new Date(ms);

				let hours = date.getHours();
				const minutes = date.getMinutes().toString().padStart(2, '0');

				const suffix = hours >= 12 ? "pm" : "am";
				const formattedHours = ((hours + 11) % 12 + 1); // convert 0–23 to 1–12

			return `${formattedHours}:${minutes} ${suffix}`;
		},
		getRandomTextColor() {
			const colors = [
				'text-red-600',
				'text-green-600',
				'text-blue-600',
				'text-yellow-600',
				'text-purple-600',
				'text-pink-600',
				'text-teal-600',
				'text-indigo-600',
				'text-gray-600',
				'text-orange-600',
				'text-lime-600',
				'text-cyan-600',
				'text-fuchsia-600',
				'text-violet-600',
				'text-rose-600',
			];
			return colors[Math.floor(Math.random() * colors.length)];
		},
		addOnlineUser( username ) {
			this.onlineUsers.push( username );
			this.colors[ username ] = this.getRandomTextColor();
		},
		setStopTimer() {
			this.typingTimeoutID = setTimeout(()=>{
				this.isTyping = false;
				this.stopTypingIndicator();
			}, 3000 );
		},
		startTypingIndicator() {
			// Stop typing indicator after 3 seconds of inactivity
				if ( !this.isTyping ) {
					this.isTyping = true;
					this.setStopTimer();
					
					// Start typing indicator
						this.wsClient.send( JSON.stringify({action:'start_typing'}) );
						console.log( 'Start typing' )
				} else {
					clearTimeout( this.typingTimeoutID );
					this.setStopTimer();
				}
		},
		stopTypingIndicator() {
			this.wsClient.send( JSON.stringify({action:'stop_typing'}) );
			console.log( 'Stop typing' )
		},
		sendMessage() {
			const message = {
				action: 'message',
				content: this.input,
			};
			this.wsClient.send( JSON.stringify(message) );
			this.input = '';

			clearTimeout( this.typingTimeoutID );
			this.isTyping = false;
		},
		wsConnect() {
			const token = this.$refs.token.getAttribute( 'data-token' );
			this.wsClient = new WebSocket( `ws://localhost:9001?token=${token}` );
			if ( this.wsClient.readyState == WebSocket.CONNECTING ) console.log("Still connecting...");
			
			this.wsClient.onopen = () => {
				console.log("Connected!");
			};
			
			this.wsClient.onmessage = (event) => {
					const message = JSON.parse( event.data );
					console.log( message );
					
				// Add new message
					if ( message.action == 'message' ) {
						this.messages.push( message );
						// Stop typing indicator from other clients
							const index = this.typingUsers.indexOf( message.username );
							if ( index >= 0 ) this.typingUsers.splice( index, 1 );
					}

				// List all of online users
					else if ( message.action == 'online_users' ) {
						this.onlineUsers = [];
						for(username of message.users) {
							this.addOnlineUser( username );
						}
					}

				// list all of messages from history
					else if ( message.action == "message_history" ) {
						console.log( 'Message history: ', message.messages );
						for(msg of message.messages) {
							this.messages.push( msg );
						}
					}

				// Start and stop typing indicator
					else if ( message.action == 'start_typing' ) this.typingUsers.push( message.username );
					else if ( message.action == 'stop_typing' ) {
						const index = this.typingUsers.indexOf( message.username );
						this.typingUsers.splice( index, 1 );
					}

				// Scroll down chat area
					this.$nextTick(() => {
						this.$refs.chat.scrollTo({
							top: this.$refs.chat.scrollHeight,
							behavior: 'smooth'
						})
					})

				// Reset user input
					this.input = '';
			};

			this.wsClient.onclose = () => {
				console.log("Disconnected. Reconnecting in 3s...");
				setTimeout(() => this.wsConnect(), 3000);
			};

			this.wsClient.onerror = (err) => {
				console.error("WebSocket error:", err);
				this.wsClient.close(); // trigger onclose then reconnect
			};
		},
	}));
});
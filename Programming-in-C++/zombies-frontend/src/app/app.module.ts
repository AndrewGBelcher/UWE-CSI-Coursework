import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule, JsonpModule } from '@angular/http';

import { routing,
         appRoutingProviders }  from './app.routing';

import { AppComponent } from './app.component';
import { AboutComponent } from './about/about.component';
import { HomeComponent } from './home/home.component';
import { OutputPanelComponent } from './output-panel/output-panel.component';
import { ConnectComponent } from './connect/connect.component';

import { SocketServiceComponent } from './socket-service/socket-service.component';

@NgModule({
  declarations: [
    AppComponent,
    AboutComponent,
    HomeComponent,
    ConnectComponent,
    OutputPanelComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    JsonpModule,
    routing
  ],
  providers: [
    appRoutingProviders,
    SocketServiceComponent
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }

FROM node:20-alpine AS base
WORKDIR /app
EXPOSE 80

FROM node:20-alpine AS build
WORKDIR /src
COPY . .
RUN npm ci
RUN npm run build

FROM base AS final
WORKDIR /app
COPY dist ./dist
COPY package.json package-lock.json ./
COPY --from=build /src/build ./build
RUN npm ci --omit=dev

ENV PORT=80

ENTRYPOINT ["npm", "start"]
